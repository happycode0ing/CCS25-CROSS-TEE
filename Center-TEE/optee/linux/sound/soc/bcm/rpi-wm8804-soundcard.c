// SPDX-License-Identifier: GPL-2.0
/*
 * rpi--wm8804.c -- ALSA SoC Raspberry Pi soundcard.
 *
 * Copyright (C) 2018 Raspberry Pi.
 *
 * Authors: Tim Gover <tim.gover@raspberrypi.org>
 *
 * Generic driver for Pi Hat WM8804 digi sounds cards
 *
 * Based upon code from:
 * justboom-digi.c
 * by Milan Neskovic <info@justboom.co>
 *
 * iqaudio_digi.c
 * by Daniel Matuschek <info@crazy-audio.com>
 *
 * allo-digione.c
 * by Baswaraj <jaikumar@cem-solutions.net>
 *
 * hifiberry-digi.c
 * Daniel Matuschek <info@crazy-audio.com>
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

#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/module.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "../codecs/wm8804.h"

struct wm8804_clk_cfg {
	unsigned int sysclk_freq;
	unsigned int mclk_freq;
	unsigned int mclk_div;
};

/* Parameters for generic functions */
struct snd_rpi_wm8804_drvdata {
	/* Required - pointer to the DAI structure */
	struct snd_soc_dai_link *dai;
	/* Required - snd_soc_card name */
	const char *card_name;
	/* Optional DT node names if card info is defined in DT */
	const char *card_name_dt;
	const char *dai_name_dt;
	const char *dai_stream_name_dt;
	/* Optional probe extension - called prior to register_card */
	int (*probe)(struct platform_device *pdev);
};

static struct gpio_desc *snd_clk44gpio;
static struct gpio_desc *snd_clk48gpio;
static int wm8804_samplerate = 0;

/* Forward declarations */
static struct snd_soc_dai_link snd_allo_digione_dai[];
static struct snd_soc_card snd_rpi_wm8804;


#define CLK_44EN_RATE 22579200UL
#define CLK_48EN_RATE 24576000UL

static unsigned int snd_rpi_wm8804_enable_clock(unsigned int samplerate)
{
	switch (samplerate) {
	case 11025:
	case 22050:
	case 44100:
	case 88200:
	case 176400:
		gpiod_set_value_cansleep(snd_clk44gpio, 1);
		gpiod_set_value_cansleep(snd_clk48gpio, 0);
		return CLK_44EN_RATE;
	default:
		gpiod_set_value_cansleep(snd_clk48gpio, 1);
		gpiod_set_value_cansleep(snd_clk44gpio, 0);
		return CLK_48EN_RATE;
	}
}

static void snd_rpi_wm8804_clk_cfg(unsigned int samplerate,
		struct wm8804_clk_cfg *clk_cfg)
{
	clk_cfg->sysclk_freq = 27000000;

	if (samplerate <= 96000 ||
	    snd_rpi_wm8804.dai_link == snd_allo_digione_dai) {
		clk_cfg->mclk_freq = samplerate * 256;
		clk_cfg->mclk_div = WM8804_MCLKDIV_256FS;
	} else {
		clk_cfg->mclk_freq = samplerate * 128;
		clk_cfg->mclk_div = WM8804_MCLKDIV_128FS;
	}

	if (!(IS_ERR(snd_clk44gpio) || IS_ERR(snd_clk48gpio)))
		clk_cfg->sysclk_freq = snd_rpi_wm8804_enable_clock(samplerate);
}

static int snd_rpi_wm8804_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = snd_soc_rtd_to_codec(rtd, 0);
	struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int sampling_freq = 1;
	int ret;
	struct wm8804_clk_cfg clk_cfg;
	int samplerate = params_rate(params);

	if (samplerate == wm8804_samplerate)
		return 0;

	/* clear until all clocks are setup properly */
	wm8804_samplerate = 0;

	snd_rpi_wm8804_clk_cfg(samplerate, &clk_cfg);

	pr_debug("%s samplerate: %d mclk_freq: %u mclk_div: %u sysclk: %u\n",
			__func__, samplerate, clk_cfg.mclk_freq,
			clk_cfg.mclk_div, clk_cfg.sysclk_freq);

	switch (samplerate) {
	case 32000:
		sampling_freq = 0x03;
		break;
	case 44100:
		sampling_freq = 0x00;
		break;
	case 48000:
		sampling_freq = 0x02;
		break;
	case 88200:
		sampling_freq = 0x08;
		break;
	case 96000:
		sampling_freq = 0x0a;
		break;
	case 176400:
		sampling_freq = 0x0c;
		break;
	case 192000:
		sampling_freq = 0x0e;
		break;
	default:
		dev_err(rtd->card->dev,
		"Failed to set WM8804 SYSCLK, unsupported samplerate %d\n",
		samplerate);
	}

	snd_soc_dai_set_clkdiv(codec_dai, WM8804_MCLK_DIV, clk_cfg.mclk_div);
	snd_soc_dai_set_pll(codec_dai, 0, 0,
			clk_cfg.sysclk_freq, clk_cfg.mclk_freq);

	ret = snd_soc_dai_set_sysclk(codec_dai, WM8804_TX_CLKSRC_PLL,
			clk_cfg.sysclk_freq, SND_SOC_CLOCK_OUT);
	if (ret < 0) {
		dev_err(rtd->card->dev,
		"Failed to set WM8804 SYSCLK: %d\n", ret);
		return ret;
	}

	wm8804_samplerate = samplerate;

	/* set sampling frequency status bits */
	snd_soc_component_update_bits(component, WM8804_SPDTX4, 0x0f,
			sampling_freq);

	return snd_soc_dai_set_bclk_ratio(cpu_dai, 64);
}

static struct snd_soc_ops snd_rpi_wm8804_ops = {
	.hw_params = snd_rpi_wm8804_hw_params,
};

SND_SOC_DAILINK_DEFS(justboom_digi,
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link snd_justboom_digi_dai[] = {
{
	.name        = "JustBoom Digi",
	.stream_name = "JustBoom Digi HiFi",
	SND_SOC_DAILINK_REG(justboom_digi),
},
};

static struct snd_rpi_wm8804_drvdata drvdata_justboom_digi = {
	.card_name            = "snd_rpi_justboom_digi",
	.dai                  = snd_justboom_digi_dai,
};

SND_SOC_DAILINK_DEFS(iqaudio_digi,
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link snd_iqaudio_digi_dai[] = {
{
	.name        = "IQAudIO Digi",
	.stream_name = "IQAudIO Digi HiFi",
	SND_SOC_DAILINK_REG(iqaudio_digi),
},
};

static struct snd_rpi_wm8804_drvdata drvdata_iqaudio_digi = {
	.card_name          = "IQAudIODigi",
	.dai                = snd_iqaudio_digi_dai,
	.card_name_dt       = "wm8804-digi,card-name",
	.dai_name_dt        = "wm8804-digi,dai-name",
	.dai_stream_name_dt = "wm8804-digi,dai-stream-name",
};

static int snd_allo_digione_probe(struct platform_device *pdev)
{
	pr_debug("%s\n", __func__);

	if (IS_ERR(snd_clk44gpio) || IS_ERR(snd_clk48gpio)) {
		dev_err(&pdev->dev, "devm_gpiod_get() failed\n");
		return -EINVAL;
	}
	return 0;
}

SND_SOC_DAILINK_DEFS(allo_digione,
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link snd_allo_digione_dai[] = {
{
	.name        = "Allo DigiOne",
	.stream_name = "Allo DigiOne HiFi",
	SND_SOC_DAILINK_REG(allo_digione),
},
};

static struct snd_rpi_wm8804_drvdata drvdata_allo_digione = {
	.card_name = "snd_allo_digione",
	.dai       = snd_allo_digione_dai,
	.probe     = snd_allo_digione_probe,
};

SND_SOC_DAILINK_DEFS(hifiberry_digi,
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link snd_hifiberry_digi_dai[] = {
{
	.name        = "HifiBerry Digi",
	.stream_name = "HifiBerry Digi HiFi",
	SND_SOC_DAILINK_REG(hifiberry_digi),
},
};

static int snd_hifiberry_digi_probe(struct platform_device *pdev)
{
	pr_debug("%s\n", __func__);

	if (IS_ERR(snd_clk44gpio) || IS_ERR(snd_clk48gpio))
		return 0;

	snd_hifiberry_digi_dai->name = "HiFiBerry Digi+ Pro";
	snd_hifiberry_digi_dai->stream_name = "HiFiBerry Digi+ Pro HiFi";
	return 0;
}

static struct snd_rpi_wm8804_drvdata drvdata_hifiberry_digi = {
	.card_name = "snd_rpi_hifiberry_digi",
	.dai       = snd_hifiberry_digi_dai,
	.probe     = snd_hifiberry_digi_probe,
};

static const struct of_device_id snd_rpi_wm8804_of_match[] = {
	{ .compatible = "justboom,justboom-digi",
		.data = (void *) &drvdata_justboom_digi },
	{ .compatible = "iqaudio,wm8804-digi",
		.data = (void *) &drvdata_iqaudio_digi },
	{ .compatible = "allo,allo-digione",
		.data = (void *) &drvdata_allo_digione },
	{ .compatible = "hifiberry,hifiberry-digi",
		.data = (void *) &drvdata_hifiberry_digi },
	{},
};

static struct snd_soc_card snd_rpi_wm8804 = {
	.driver_name  = "RPi-WM8804",
	.owner        = THIS_MODULE,
	.dai_link     = NULL,
	.num_links    = 1,
};

static int snd_rpi_wm8804_probe(struct platform_device *pdev)
{
	int ret = 0;
	const struct of_device_id *of_id;

	snd_rpi_wm8804.dev = &pdev->dev;
	of_id = of_match_node(snd_rpi_wm8804_of_match, pdev->dev.of_node);

	if (pdev->dev.of_node && of_id->data) {
		struct device_node *i2s_node;
		struct snd_rpi_wm8804_drvdata *drvdata =
			(struct snd_rpi_wm8804_drvdata *) of_id->data;
		struct snd_soc_dai_link *dai = drvdata->dai;

		snd_soc_card_set_drvdata(&snd_rpi_wm8804, drvdata);

		if (!dai->ops)
			dai->ops = &snd_rpi_wm8804_ops;
		if (!dai->codecs->dai_name)
			dai->codecs->dai_name = "wm8804-spdif";
		if (!dai->codecs->name)
			dai->codecs->name = "wm8804.1-003b";
		if (!dai->dai_fmt)
			dai->dai_fmt = SND_SOC_DAIFMT_I2S |
				SND_SOC_DAIFMT_NB_NF |
				SND_SOC_DAIFMT_CBM_CFM;

		snd_rpi_wm8804.dai_link = dai;
		i2s_node = of_parse_phandle(pdev->dev.of_node,
				"i2s-controller", 0);
		if (!i2s_node) {
			pr_err("Failed to find i2s-controller DT node\n");
			return -ENODEV;
		}

		snd_rpi_wm8804.name = drvdata->card_name;

		/* If requested by in drvdata get card & DAI names from DT */
		if (drvdata->card_name_dt)
			of_property_read_string(i2s_node,
					drvdata->card_name_dt,
					&snd_rpi_wm8804.name);

		if (drvdata->dai_name_dt)
			of_property_read_string(i2s_node,
					drvdata->dai_name_dt,
					&dai->name);

		if (drvdata->dai_stream_name_dt)
			of_property_read_string(i2s_node,
					drvdata->dai_stream_name_dt,
					&dai->stream_name);

		dai->cpus->of_node = i2s_node;
		dai->platforms->of_node = i2s_node;

		/*
		 * clk44gpio and clk48gpio are not required by all cards so
		 * don't check the error status.
		 */
		snd_clk44gpio =
			devm_gpiod_get(&pdev->dev, "clock44", GPIOD_OUT_LOW);

		snd_clk48gpio =
			devm_gpiod_get(&pdev->dev, "clock48", GPIOD_OUT_LOW);

		if (drvdata->probe) {
			ret = drvdata->probe(pdev);
			if (ret < 0) {
				dev_err(&pdev->dev, "Custom probe failed %d\n",
						ret);
				return ret;
			}
		}

		pr_debug("%s card: %s dai: %s stream: %s\n", __func__,
				snd_rpi_wm8804.name,
				dai->name, dai->stream_name);
	}

	ret = devm_snd_soc_register_card(&pdev->dev, &snd_rpi_wm8804);
	if (ret && ret != -EPROBE_DEFER)
		dev_err(&pdev->dev, "Failed to register card %d\n", ret);

	return ret;
}

static struct platform_driver snd_rpi_wm8804_driver = {
	.driver = {
		.name           = "snd-rpi-wm8804",
		.owner          = THIS_MODULE,
		.of_match_table = snd_rpi_wm8804_of_match,
	},
	.probe  = snd_rpi_wm8804_probe,
};
MODULE_DEVICE_TABLE(of, snd_rpi_wm8804_of_match);

module_platform_driver(snd_rpi_wm8804_driver);

MODULE_AUTHOR("Tim Gover <tim.gover@raspberrypi.org>");
MODULE_DESCRIPTION("ASoC Raspberry Pi Hat generic digi driver for WM8804 based cards");
MODULE_LICENSE("GPL v2");
