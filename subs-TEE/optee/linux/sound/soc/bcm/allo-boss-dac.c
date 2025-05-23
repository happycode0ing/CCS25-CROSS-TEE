/*
 * ALSA ASoC Machine Driver for Allo Boss DAC
 *
 * Author:	Baswaraj K <jaikumar@cem-solutions.net>
 *		Copyright 2017
 *		based on code by Daniel Matuschek,
 *				 Stuart MacLean <stuart@hifiberry.com>
 *		based on code by Florian Meier <florian.meier@koalo.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/module.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/delay.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include "../codecs/pcm512x.h"

#define ALLO_BOSS_NOCLOCK 0
#define ALLO_BOSS_CLK44EN 1
#define ALLO_BOSS_CLK48EN 2

struct pcm512x_priv {
	struct regmap *regmap;
	struct clk *sclk;
};

static struct gpio_desc *mute_gpio;

/* Clock rate of CLK44EN attached to GPIO6 pin */
#define CLK_44EN_RATE 45158400UL
/* Clock rate of CLK48EN attached to GPIO3 pin */
#define CLK_48EN_RATE 49152000UL

static bool slave;
static bool snd_soc_allo_boss_master;
static bool digital_gain_0db_limit = true;

static void snd_allo_boss_select_clk(struct snd_soc_component *component,
	int clk_id)
{
	switch (clk_id) {
	case ALLO_BOSS_NOCLOCK:
		snd_soc_component_update_bits(component, PCM512x_GPIO_CONTROL_1, 0x24, 0x00);
		break;
	case ALLO_BOSS_CLK44EN:
		snd_soc_component_update_bits(component, PCM512x_GPIO_CONTROL_1, 0x24, 0x20);
		break;
	case ALLO_BOSS_CLK48EN:
		snd_soc_component_update_bits(component, PCM512x_GPIO_CONTROL_1, 0x24, 0x04);
		break;
	}
}

static void snd_allo_boss_clk_gpio(struct snd_soc_component *component)
{
	snd_soc_component_update_bits(component, PCM512x_GPIO_EN, 0x24, 0x24);
	snd_soc_component_update_bits(component, PCM512x_GPIO_OUTPUT_3, 0x0f, 0x02);
	snd_soc_component_update_bits(component, PCM512x_GPIO_OUTPUT_6, 0x0f, 0x02);
}

static bool snd_allo_boss_is_sclk(struct snd_soc_component *component)
{
	unsigned int sck;

	sck = snd_soc_component_read(component, PCM512x_RATE_DET_4);
	return (!(sck & 0x40));
}

static bool snd_allo_boss_is_sclk_sleep(
	struct snd_soc_component *component)
{
	msleep(2);
	return snd_allo_boss_is_sclk(component);
}

static bool snd_allo_boss_is_master_card(struct snd_soc_component *component)
{
	bool isClk44EN, isClk48En, isNoClk;

	snd_allo_boss_clk_gpio(component);

	snd_allo_boss_select_clk(component, ALLO_BOSS_CLK44EN);
	isClk44EN = snd_allo_boss_is_sclk_sleep(component);

	snd_allo_boss_select_clk(component, ALLO_BOSS_NOCLOCK);
	isNoClk = snd_allo_boss_is_sclk_sleep(component);

	snd_allo_boss_select_clk(component, ALLO_BOSS_CLK48EN);
	isClk48En = snd_allo_boss_is_sclk_sleep(component);

	return (isClk44EN && isClk48En && !isNoClk);
}

static int snd_allo_boss_clk_for_rate(int sample_rate)
{
	int type;

	switch (sample_rate) {
	case 11025:
	case 22050:
	case 44100:
	case 88200:
	case 176400:
	case 352800:
		type = ALLO_BOSS_CLK44EN;
		break;
	default:
		type = ALLO_BOSS_CLK48EN;
		break;
	}
	return type;
}

static void snd_allo_boss_set_sclk(struct snd_soc_component *component,
	int sample_rate)
{
	struct pcm512x_priv *pcm512x = snd_soc_component_get_drvdata(component);

	if (!IS_ERR(pcm512x->sclk)) {
		int ctype;

		ctype =	snd_allo_boss_clk_for_rate(sample_rate);
		clk_set_rate(pcm512x->sclk, (ctype == ALLO_BOSS_CLK44EN)
				? CLK_44EN_RATE : CLK_48EN_RATE);
		snd_allo_boss_select_clk(component, ctype);
	}
}

static int snd_allo_boss_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;
	struct pcm512x_priv *priv = snd_soc_component_get_drvdata(component);

	if (slave)
		snd_soc_allo_boss_master = false;
	else
		snd_soc_allo_boss_master =
			snd_allo_boss_is_master_card(component);

	if (snd_soc_allo_boss_master) {
		struct snd_soc_dai_link *dai = rtd->dai_link;

		dai->name = "BossDAC";
		dai->stream_name = "Boss DAC HiFi [Master]";
		dai->dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF
			| SND_SOC_DAIFMT_CBM_CFM;

		snd_soc_component_update_bits(component, PCM512x_BCLK_LRCLK_CFG, 0x31, 0x11);
		snd_soc_component_update_bits(component, PCM512x_MASTER_MODE, 0x03, 0x03);
		snd_soc_component_update_bits(component, PCM512x_MASTER_CLKDIV_2, 0x7f, 63);
		/*
		* Default sclk to CLK_48EN_RATE, otherwise codec
		*  pcm512x_dai_startup_master method could call
		*  snd_pcm_hw_constraint_ratnums using CLK_44EN/64
		*  which will mask 384k sample rate.
		*/
		if (!IS_ERR(priv->sclk))
			clk_set_rate(priv->sclk, CLK_48EN_RATE);
	} else {
		priv->sclk = ERR_PTR(-ENOENT);
	}

	snd_soc_component_update_bits(component, PCM512x_GPIO_EN, 0x08, 0x08);
	snd_soc_component_update_bits(component, PCM512x_GPIO_OUTPUT_4, 0x0f, 0x02);
	snd_soc_component_update_bits(component, PCM512x_GPIO_CONTROL_1, 0x08, 0x08);

	if (digital_gain_0db_limit) {
		int ret;
		struct snd_soc_card *card = rtd->card;

		ret = snd_soc_limit_volume(card, "Digital Playback Volume",
				207);
		if (ret < 0)
			dev_warn(card->dev, "Failed to set volume limit: %d\n",
					ret);
	}

	return 0;
}

static int snd_allo_boss_update_rate_den(
	struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;
	struct pcm512x_priv *pcm512x = snd_soc_component_get_drvdata(component);
	struct snd_ratnum *rats_no_pll;
	unsigned int num = 0, den = 0;
	int err;

	rats_no_pll = devm_kzalloc(rtd->dev, sizeof(*rats_no_pll), GFP_KERNEL);
	if (!rats_no_pll)
		return -ENOMEM;

	rats_no_pll->num = clk_get_rate(pcm512x->sclk) / 64;
	rats_no_pll->den_min = 1;
	rats_no_pll->den_max = 128;
	rats_no_pll->den_step = 1;

	err = snd_interval_ratnum(hw_param_interval(params,
		SNDRV_PCM_HW_PARAM_RATE), 1, rats_no_pll, &num, &den);
	if (err >= 0 && den) {
		params->rate_num = num;
		params->rate_den = den;
	}

	devm_kfree(rtd->dev, rats_no_pll);
	return 0;
}

static void snd_allo_boss_gpio_mute(struct snd_soc_card *card)
{
	if (mute_gpio)
		gpiod_set_value_cansleep(mute_gpio, 1);
}

static void snd_allo_boss_gpio_unmute(struct snd_soc_card *card)
{
	if (mute_gpio)
		gpiod_set_value_cansleep(mute_gpio, 0);
}

static int snd_allo_boss_set_bias_level(struct snd_soc_card *card,
	struct snd_soc_dapm_context *dapm, enum snd_soc_bias_level level)
{
	struct snd_soc_pcm_runtime *rtd;
	struct snd_soc_dai *codec_dai;

	rtd = snd_soc_get_pcm_runtime(card, &card->dai_link[0]);
	codec_dai = snd_soc_rtd_to_codec(rtd, 0);

	if (dapm->dev != codec_dai->dev)
		return 0;

	switch (level) {
	case SND_SOC_BIAS_PREPARE:
		if (dapm->bias_level != SND_SOC_BIAS_STANDBY)
			break;
		/* UNMUTE DAC */
		snd_allo_boss_gpio_unmute(card);
		break;

	case SND_SOC_BIAS_STANDBY:
		if (dapm->bias_level != SND_SOC_BIAS_PREPARE)
			break;
		/* MUTE DAC */
		snd_allo_boss_gpio_mute(card);
		break;

	default:
		break;
	}

	return 0;
}

static int snd_allo_boss_hw_params(
	struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	int ret = 0;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	int channels = params_channels(params);
	int width = snd_pcm_format_physical_width(params_format(params));

	if (snd_soc_allo_boss_master) {
		struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;

		snd_allo_boss_set_sclk(component,
			params_rate(params));

		ret = snd_allo_boss_update_rate_den(
			substream, params);
		if (ret)
			return ret;
	}

	ret = snd_soc_dai_set_bclk_ratio(snd_soc_rtd_to_cpu(rtd, 0), channels * width);
	if (ret)
		return ret;
	ret = snd_soc_dai_set_bclk_ratio(snd_soc_rtd_to_codec(rtd, 0), channels * width);
	return ret;
}

static int snd_allo_boss_startup(
	struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;
	struct snd_soc_card *card = rtd->card;

	snd_soc_component_update_bits(component, PCM512x_GPIO_CONTROL_1, 0x08, 0x08);
	snd_allo_boss_gpio_mute(card);

	if (snd_soc_allo_boss_master) {
		struct pcm512x_priv *priv = snd_soc_component_get_drvdata(component);
		/*
		 * Default sclk to CLK_48EN_RATE, otherwise codec
		 * pcm512x_dai_startup_master method could call
		 * snd_pcm_hw_constraint_ratnums using CLK_44EN/64
		 * which will mask 384k sample rate.
		 */
		if (!IS_ERR(priv->sclk))
			clk_set_rate(priv->sclk, CLK_48EN_RATE);
	}

	return 0;
}

static void snd_allo_boss_shutdown(
	struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;

	snd_soc_component_update_bits(component, PCM512x_GPIO_CONTROL_1, 0x08, 0x00);
}

static int snd_allo_boss_prepare(
	struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *card = rtd->card;

	snd_allo_boss_gpio_unmute(card);
	return 0;
}
/* machine stream operations */
static struct snd_soc_ops snd_allo_boss_ops = {
	.hw_params = snd_allo_boss_hw_params,
	.startup = snd_allo_boss_startup,
	.shutdown = snd_allo_boss_shutdown,
	.prepare = snd_allo_boss_prepare,
};

SND_SOC_DAILINK_DEFS(allo_boss,
	DAILINK_COMP_ARRAY(COMP_CPU("bcm2708-i2s.0")),
	DAILINK_COMP_ARRAY(COMP_CODEC("pcm512x.1-004d", "pcm512x-hifi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("bcm2708-i2s.0")));

static struct snd_soc_dai_link snd_allo_boss_dai[] = {
{
	.name		= "Boss DAC",
	.stream_name	= "Boss DAC HiFi",
	.dai_fmt	= SND_SOC_DAIFMT_I2S |
			  SND_SOC_DAIFMT_NB_NF |
			  SND_SOC_DAIFMT_CBS_CFS,
	.ops		= &snd_allo_boss_ops,
	.init		= snd_allo_boss_init,
	SND_SOC_DAILINK_REG(allo_boss),
},
};

/* audio machine driver */
static struct snd_soc_card snd_allo_boss = {
	.name         = "BossDAC",
	.owner        = THIS_MODULE,
	.dai_link     = snd_allo_boss_dai,
	.num_links    = ARRAY_SIZE(snd_allo_boss_dai),
};

static int snd_allo_boss_probe(struct platform_device *pdev)
{
	int ret = 0;

	snd_allo_boss.dev = &pdev->dev;

	if (pdev->dev.of_node) {
		struct device_node *i2s_node;
		struct snd_soc_dai_link *dai;

		dai = &snd_allo_boss_dai[0];
		i2s_node = of_parse_phandle(pdev->dev.of_node,
					    "i2s-controller", 0);

		if (i2s_node) {
			dai->cpus->dai_name = NULL;
			dai->cpus->of_node = i2s_node;
			dai->platforms->name = NULL;
			dai->platforms->of_node = i2s_node;
		}

		digital_gain_0db_limit = !of_property_read_bool(
			pdev->dev.of_node, "allo,24db_digital_gain");
		slave = of_property_read_bool(pdev->dev.of_node,
						"allo,slave");

		mute_gpio = devm_gpiod_get_optional(&pdev->dev, "mute",
							GPIOD_OUT_LOW);
		if (IS_ERR(mute_gpio)) {
			ret = PTR_ERR(mute_gpio);
			dev_err(&pdev->dev,
				"failed to get mute gpio: %d\n", ret);
			return ret;
		}

		if (mute_gpio)
			snd_allo_boss.set_bias_level =
				snd_allo_boss_set_bias_level;

		ret = snd_soc_register_card(&snd_allo_boss);
		if (ret) {
			dev_err(&pdev->dev,
				"snd_soc_register_card() failed: %d\n", ret);
			return ret;
		}

		if (mute_gpio)
			snd_allo_boss_gpio_mute(&snd_allo_boss);

		return 0;
	}

	return -EINVAL;
}

static int snd_allo_boss_remove(struct platform_device *pdev)
{
	snd_allo_boss_gpio_mute(&snd_allo_boss);
	snd_soc_unregister_card(&snd_allo_boss);
	return 0;
}

static const struct of_device_id snd_allo_boss_of_match[] = {
	{ .compatible = "allo,boss-dac", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, snd_allo_boss_of_match);

static struct platform_driver snd_allo_boss_driver = {
	.driver = {
		.name   = "snd-allo-boss-dac",
		.owner  = THIS_MODULE,
		.of_match_table = snd_allo_boss_of_match,
	},
	.probe          = snd_allo_boss_probe,
	.remove         = snd_allo_boss_remove,
};

module_platform_driver(snd_allo_boss_driver);

MODULE_AUTHOR("Baswaraj K <jaikumar@cem-solutions.net>");
MODULE_DESCRIPTION("ALSA ASoC Machine Driver for Allo Boss DAC");
MODULE_LICENSE("GPL v2");
