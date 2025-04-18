/*
 * ASoC Driver for Fe-Pi Audio Sound Card
 *
 * Author:	Henry Kupis <kuupaz@gmail.com>
 *		Copyright 2016
 *		based on code by Florian Meier <florian.meier@koalo.de>
 *		based on code by Shawn Guo <shawn.guo@linaro.org>
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
#include <linux/platform_device.h>
#include <linux/io.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

#include "../codecs/sgtl5000.h"

static int snd_fe_pi_audio_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_card *card = rtd->card;
	struct snd_soc_component *component = snd_soc_rtd_to_codec(rtd, 0)->component;

	snd_soc_dapm_force_enable_pin(&card->dapm, "LO");
	snd_soc_dapm_force_enable_pin(&card->dapm, "ADC");
	snd_soc_dapm_force_enable_pin(&card->dapm, "DAC");
	snd_soc_dapm_force_enable_pin(&card->dapm, "HP");
	snd_soc_component_update_bits(component, SGTL5000_CHIP_ANA_POWER,
			SGTL5000_VAG_POWERUP, SGTL5000_VAG_POWERUP);

	return 0;
}

static int snd_fe_pi_audio_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct device *dev = rtd->card->dev;
	struct snd_soc_dai *codec_dai = snd_soc_rtd_to_codec(rtd, 0);

	int ret;

	/* Set SGTL5000's SYSCLK */
	ret = snd_soc_dai_set_sysclk(codec_dai, SGTL5000_SYSCLK, 12288000, SND_SOC_CLOCK_IN);
	if (ret) {
		dev_err(dev, "could not set codec driver clock params\n");
		return ret;
	}

	return 0;
}


static struct snd_soc_ops snd_fe_pi_audio_ops = {
	.hw_params = snd_fe_pi_audio_hw_params,
};

SND_SOC_DAILINK_DEFS(fe_pi,
	DAILINK_COMP_ARRAY(COMP_CPU("bcm2708-i2s.0")),
	DAILINK_COMP_ARRAY(COMP_CODEC("sgtl5000.1-000a", "sgtl5000")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("bcm2708-i2s.0")));

static struct snd_soc_dai_link snd_fe_pi_audio_dai[] = {
	{
		.name		= "FE-PI",
		.stream_name	= "Fe-Pi HiFi",
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
					SND_SOC_DAIFMT_CBM_CFM,
		.ops		= &snd_fe_pi_audio_ops,
		.init		= snd_fe_pi_audio_init,
		SND_SOC_DAILINK_REG(fe_pi),
	},
};

static const struct snd_soc_dapm_route fe_pi_audio_dapm_routes[] = {
	{"ADC", NULL, "Mic Bias"},
};


static struct snd_soc_card fe_pi_audio = {
	.name         = "Fe-Pi Audio",
	.owner        = THIS_MODULE,
	.dai_link     = snd_fe_pi_audio_dai,
	.num_links    = ARRAY_SIZE(snd_fe_pi_audio_dai),

	.dapm_routes = fe_pi_audio_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(fe_pi_audio_dapm_routes),
};

static int snd_fe_pi_audio_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct snd_soc_card *card = &fe_pi_audio;
	struct device_node *np = pdev->dev.of_node;
	struct device_node *i2s_node;
	struct snd_soc_dai_link *dai = &snd_fe_pi_audio_dai[0];

	fe_pi_audio.dev = &pdev->dev;

	i2s_node = of_parse_phandle(np, "i2s-controller", 0);
	if (!i2s_node) {
		dev_err(&pdev->dev, "i2s_node phandle missing or invalid\n");
		return -EINVAL;
	}

	dai->cpus->dai_name = NULL;
	dai->cpus->of_node = i2s_node;
	dai->platforms->name = NULL;
	dai->platforms->of_node = i2s_node;

	of_node_put(i2s_node);

	card->dev = &pdev->dev;
	platform_set_drvdata(pdev, card);

	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret && ret != -EPROBE_DEFER)
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n", ret);

	return ret;
}

static const struct of_device_id snd_fe_pi_audio_of_match[] = {
	{ .compatible = "fe-pi,fe-pi-audio", },
	{},
};
MODULE_DEVICE_TABLE(of, snd_fe_pi_audio_of_match);

static struct platform_driver snd_fe_pi_audio_driver = {
        .driver = {
                .name   = "snd-fe-pi-audio",
                .owner  = THIS_MODULE,
                .of_match_table = snd_fe_pi_audio_of_match,
        },
        .probe          = snd_fe_pi_audio_probe,
};

module_platform_driver(snd_fe_pi_audio_driver);

MODULE_AUTHOR("Henry Kupis <fe-pi@cox.net>");
MODULE_DESCRIPTION("ASoC Driver for Fe-Pi Audio");
MODULE_LICENSE("GPL v2");
