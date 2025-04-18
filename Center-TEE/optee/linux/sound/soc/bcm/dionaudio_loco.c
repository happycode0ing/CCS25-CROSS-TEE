/*
 * ASoC Driver for Dion Audio LOCO DAC-AMP
 *
 * Author:      Miquel Blauw <info@dionaudio.nl>
 *              Copyright 2016
 *
 * Based on the software of the RPi-DAC writen by Florian Meier
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

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

static int snd_rpi_dionaudio_loco_hw_params(
	struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);

	unsigned int sample_bits =
		snd_pcm_format_physical_width(params_format(params));

	return snd_soc_dai_set_bclk_ratio(cpu_dai, sample_bits * 2);
}

/* machine stream operations */
static struct snd_soc_ops snd_rpi_dionaudio_loco_ops = {
	.hw_params = snd_rpi_dionaudio_loco_hw_params,
};

SND_SOC_DAILINK_DEFS(dionaudio_loco,
	DAILINK_COMP_ARRAY(COMP_CPU("bcm2708-i2s.0")),
	DAILINK_COMP_ARRAY(COMP_CODEC("pcm5102a-codec", "pcm5102a-hifi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("bcm2708-i2s.0")));

static struct snd_soc_dai_link snd_rpi_dionaudio_loco_dai[] = {
{
	.name		= "DionAudio LOCO",
	.stream_name	= "DionAudio LOCO DAC-AMP",
	.dai_fmt	= SND_SOC_DAIFMT_I2S |
			  SND_SOC_DAIFMT_NB_NF |
			  SND_SOC_DAIFMT_CBS_CFS,
	.ops		= &snd_rpi_dionaudio_loco_ops,
	SND_SOC_DAILINK_REG(dionaudio_loco),
},
};

/* audio machine driver */
static struct snd_soc_card snd_rpi_dionaudio_loco = {
	.name		= "snd_rpi_dionaudio_loco",
	.dai_link	= snd_rpi_dionaudio_loco_dai,
	.num_links	= ARRAY_SIZE(snd_rpi_dionaudio_loco_dai),
};

static int snd_rpi_dionaudio_loco_probe(struct platform_device *pdev)
{
	struct device_node *np;
	int ret = 0;

	snd_rpi_dionaudio_loco.dev = &pdev->dev;

	np = pdev->dev.of_node;
	if (np) {
		struct snd_soc_dai_link *dai = &snd_rpi_dionaudio_loco_dai[0];
		struct device_node *i2s_np;

		i2s_np = of_parse_phandle(np, "i2s-controller", 0);
		if (i2s_np) {
			dai->cpus->dai_name = NULL;
			dai->cpus->of_node = i2s_np;
			dai->platforms->name = NULL;
			dai->platforms->of_node = i2s_np;
		}
	}

	ret = devm_snd_soc_register_card(&pdev->dev, &snd_rpi_dionaudio_loco);
	if (ret && ret != -EPROBE_DEFER)
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n",
			ret);

	return ret;
}

static const struct of_device_id snd_rpi_dionaudio_loco_of_match[] = {
	{ .compatible = "dionaudio,loco-pcm5242-tpa3118", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, snd_rpi_dionaudio_loco_of_match);

static struct platform_driver snd_rpi_dionaudio_loco_driver = {
	.driver = {
		.name		= "snd-dionaudio-loco",
		.owner		= THIS_MODULE,
		.of_match_table	= snd_rpi_dionaudio_loco_of_match,
	},
	.probe  = snd_rpi_dionaudio_loco_probe,
};

module_platform_driver(snd_rpi_dionaudio_loco_driver);

MODULE_AUTHOR("Miquel Blauw <info@dionaudio.nl>");
MODULE_DESCRIPTION("ASoC Driver for DionAudio LOCO");
MODULE_LICENSE("GPL v2");
