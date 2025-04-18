/*
 * ASoC Driver for ChipDip DAC
 *
 * Author:	Evgenij Sapunov
 *		Copyright 2021
 *		based on code by Milan Neskovic <info@justboom.co>
 *		based on code by Jaikumar <jaikumar@cem-solutions.net>
 *
 * Thanks to Phil Elwell (pelwell) for help.
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
#include <linux/delay.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

#define SR_BIT_0                  0 //sample rate bits
#define SR_BIT_1                  1
#define SR_BIT_2                  2
#define BD_BIT_0                  3 //bit depth bits
#define BD_BIT_1                  4

#define SAMPLE_RATE_MASK_44_1     0
#define SAMPLE_RATE_MASK_48       (1 << SR_BIT_0)
#define SAMPLE_RATE_MASK_88_2     ((1 << SR_BIT_2) | (1 << SR_BIT_1))
#define SAMPLE_RATE_MASK_96       (1 << SR_BIT_1)
#define SAMPLE_RATE_MASK_176_4    ((1 << SR_BIT_2) | (1 << SR_BIT_1) | (1 << SR_BIT_0))
#define SAMPLE_RATE_MASK_192      ((1 << SR_BIT_1) | (1 << SR_BIT_0))
#define SAMPLE_RATE_MASK          ((1 << SR_BIT_2) | (1 << SR_BIT_1) | (1 << SR_BIT_0))

#define BIT_DEPTH_MASK_16         0
#define BIT_DEPTH_MASK_24         (1 << BD_BIT_0)
#define BIT_DEPTH_MASK_32         (1 << BD_BIT_1)
#define BIT_DEPTH_MASK            ((1 << BD_BIT_1) | (1 << BD_BIT_0))

#define MUTE_ACTIVE               0
#define MUTE_NOT_ACTIVE           1

#define HW_PARAMS_GPIO_COUNT      5

static struct gpio_desc *mute_gpio;
static struct gpio_desc *sdwn_gpio;
static struct gpio_desc *hw_params_gpios[HW_PARAMS_GPIO_COUNT];
static int current_width;
static int current_rate;

static void snd_rpi_chipdip_dac_gpio_array_set(int value);
static void snd_rpi_chipdip_dac_gpio_set(struct gpio_desc *gpio_item, int value);

static void snd_rpi_chipdip_dac_gpio_array_set(int value)
{
	int i = 0;

	for (i = 0; i < HW_PARAMS_GPIO_COUNT; i++)
		snd_rpi_chipdip_dac_gpio_set(hw_params_gpios[i], ((value >> i) & 1));
}

static void snd_rpi_chipdip_dac_gpio_set(struct gpio_desc *gpio_item, int value)
{
	if (gpio_item)
		gpiod_set_value_cansleep(gpio_item, value);
}

static int snd_rpi_chipdip_dac_init(struct snd_soc_pcm_runtime *rtd)
{
	return 0;
}

static int snd_rpi_chipdip_dac_hw_params(struct snd_pcm_substream *substream,
					 struct snd_pcm_hw_params *params)
{
	int ret = 0;
	int gpio_change_pending = 0;
	int sample_rate_state = 0;
	int bit_depth_state = 0;
	int param_value = params_width(params);
	struct snd_soc_pcm_runtime *rtd = substream->private_data;

	ret = snd_soc_dai_set_bclk_ratio(snd_soc_rtd_to_cpu(rtd, 0), 2 * 32);

	if (current_width != param_value) {
		current_width = param_value;
		gpio_change_pending = 1;

		switch (param_value) {
		case 16:
			bit_depth_state = BIT_DEPTH_MASK_16;
			break;
		case 24:
			bit_depth_state = BIT_DEPTH_MASK_24;
			break;
		case 32:
			bit_depth_state = BIT_DEPTH_MASK_32;
			break;
		default:
			return -EINVAL;
		}
	}

	param_value = params_rate(params);
	if (current_rate != param_value) {
		current_rate = param_value;
		gpio_change_pending = 1;

		switch (param_value) {
		case 44100:
			sample_rate_state = SAMPLE_RATE_MASK_44_1;
			break;
		case 48000:
			sample_rate_state = SAMPLE_RATE_MASK_48;
			break;
		case 88200:
			sample_rate_state = SAMPLE_RATE_MASK_88_2;
			break;
		case 96000:
			sample_rate_state = SAMPLE_RATE_MASK_96;
			break;
		case 176400:
			sample_rate_state = SAMPLE_RATE_MASK_176_4;
			break;
		case 192000:
			sample_rate_state = SAMPLE_RATE_MASK_192;
			break;
		default:
			return -EINVAL;
		}
	}

	if (gpio_change_pending) {
		snd_rpi_chipdip_dac_gpio_set(mute_gpio, MUTE_ACTIVE);
		snd_rpi_chipdip_dac_gpio_array_set(bit_depth_state | sample_rate_state);
		msleep(300);
		snd_rpi_chipdip_dac_gpio_set(mute_gpio, MUTE_NOT_ACTIVE);
	}

	return ret;
}

static int snd_rpi_chipdip_dac_startup(struct snd_pcm_substream *substream)
{
	return 0;
}

static void snd_rpi_chipdip_dac_shutdown(struct snd_pcm_substream *substream)
{

}

/* machine stream operations */
static struct snd_soc_ops snd_rpi_chipdip_dac_ops = {
	.hw_params = snd_rpi_chipdip_dac_hw_params,
	.startup = snd_rpi_chipdip_dac_startup,
	.shutdown = snd_rpi_chipdip_dac_shutdown,
};

SND_SOC_DAILINK_DEFS(hifi,
	DAILINK_COMP_ARRAY(COMP_CPU("bcm2708-i2s.0")),
	DAILINK_COMP_ARRAY(COMP_CODEC("spdif-transmitter", "dit-hifi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("bcm2708-i2s.0")));

static struct snd_soc_dai_link snd_rpi_chipdip_dac_dai[] = {
{
	.name		= "ChipDip DAC",
	.stream_name	= "ChipDip DAC HiFi",
	.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				SND_SOC_DAIFMT_CBM_CFM,
	.ops		= &snd_rpi_chipdip_dac_ops,
	.init		= snd_rpi_chipdip_dac_init,
	SND_SOC_DAILINK_REG(hifi),
},
};

/* audio machine driver */
static struct snd_soc_card snd_rpi_chipdip_dac = {
	.name         = "ChipDipDAC",
	.driver_name  = "ChipdipDac",
	.owner        = THIS_MODULE,
	.dai_link     = snd_rpi_chipdip_dac_dai,
	.num_links    = ARRAY_SIZE(snd_rpi_chipdip_dac_dai),
};

static int snd_rpi_chipdip_dac_probe(struct platform_device *pdev)
{
	int ret = 0;
	int i = 0;

	snd_rpi_chipdip_dac.dev = &pdev->dev;

	if (pdev->dev.of_node) {
		struct device_node *i2s_node;
		struct snd_soc_dai_link *dai = &snd_rpi_chipdip_dac_dai[0];
		i2s_node = of_parse_phandle(pdev->dev.of_node,
					"i2s-controller", 0);

		if (i2s_node) {
			dai->cpus->dai_name = NULL;
			dai->cpus->of_node = i2s_node;
			dai->platforms->name = NULL;
			dai->platforms->of_node = i2s_node;
		}
	}

	hw_params_gpios[SR_BIT_0] = devm_gpiod_get_optional(&pdev->dev, "sr0", GPIOD_OUT_LOW);
	hw_params_gpios[SR_BIT_1] = devm_gpiod_get_optional(&pdev->dev, "sr1", GPIOD_OUT_LOW);
	hw_params_gpios[SR_BIT_2] = devm_gpiod_get_optional(&pdev->dev, "sr2", GPIOD_OUT_LOW);
	hw_params_gpios[BD_BIT_0] = devm_gpiod_get_optional(&pdev->dev, "res0", GPIOD_OUT_LOW);
	hw_params_gpios[BD_BIT_1] = devm_gpiod_get_optional(&pdev->dev, "res1", GPIOD_OUT_LOW);
	mute_gpio = devm_gpiod_get_optional(&pdev->dev, "mute", GPIOD_OUT_LOW);
	sdwn_gpio = devm_gpiod_get_optional(&pdev->dev, "sdwn", GPIOD_OUT_HIGH);

	for (i = 0; i < HW_PARAMS_GPIO_COUNT; i++) {
		if (IS_ERR(hw_params_gpios[i])) {
			ret = PTR_ERR(hw_params_gpios[i]);
			dev_err(&pdev->dev, "failed to get hw_params gpio: %d\n", ret);
			return ret;
		}
	}

	if (IS_ERR(mute_gpio)) {
		ret = PTR_ERR(mute_gpio);
		dev_err(&pdev->dev, "failed to get mute gpio: %d\n", ret);
		return ret;
	}

	if (IS_ERR(sdwn_gpio)) {
		ret = PTR_ERR(sdwn_gpio);
		dev_err(&pdev->dev, "failed to get sdwn gpio: %d\n", ret);
		return ret;
	}

	snd_rpi_chipdip_dac_gpio_set(sdwn_gpio, 1);

	ret = devm_snd_soc_register_card(&pdev->dev, &snd_rpi_chipdip_dac);
	if (ret && ret != -EPROBE_DEFER)
		dev_err(&pdev->dev,
			"snd_soc_register_card() failed: %d\n", ret);

	return ret;
}

static const struct of_device_id snd_rpi_chipdip_dac_of_match[] = {
	{ .compatible = "chipdip,chipdip-dac", },
	{},
};
MODULE_DEVICE_TABLE(of, snd_rpi_chipdip_dac_of_match);

static struct platform_driver snd_rpi_chipdip_dac_driver = {
	.driver = {
		.name   = "snd-rpi-chipdip-dac",
		.owner  = THIS_MODULE,
		.of_match_table = snd_rpi_chipdip_dac_of_match,
	},
	.probe          = snd_rpi_chipdip_dac_probe,
};

module_platform_driver(snd_rpi_chipdip_dac_driver);

MODULE_AUTHOR("Evgenij Sapunov <evgenij.sapunov@chipdip.ru>");
MODULE_DESCRIPTION("ASoC Driver for ChipDip DAC");
MODULE_LICENSE("GPL v2");
