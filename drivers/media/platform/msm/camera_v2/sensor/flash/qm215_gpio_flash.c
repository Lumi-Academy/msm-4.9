/* Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/export.h>
#include "msm_camera_io_util.h"
#include "msm_flash.h"

#define FLASH_NAME "qcom,gpio-flash"

#undef CDBG
//#define CDBG(fmt, args...) pr_debug(fmt, ##args)
#define CDBG(fmt, args...) pr_info(fmt, ##args)

static const struct of_device_id msm_gpio_flash_dt_match[] = {
	{.compatible = "qcom,qm215-gpio-flash", .data = NULL},
	{}
};
static struct msm_flash_table qm215_gpio_flash_table;
MODULE_DEVICE_TABLE(of, msm_flash_dt_match);

#if 1
static int qm215_flash_sgm37891_cfg(uint16_t gpio_id, uint16_t level)
{
	int i;
	CDBG("enter 37891 flash sprd_flash_sgm37891_cfg gpio_id = %d, level = %d,\n", gpio_id, level);

	gpio_set_value_cansleep(gpio_id, GPIO_OUT_LOW);
	usleep_range(20, 20);
	gpio_set_value_cansleep(gpio_id,GPIO_OUT_HIGH);
	usleep_range(300, 300);
	gpio_set_value_cansleep(gpio_id, GPIO_OUT_LOW);
	usleep_range(45, 45);

	/* for level > 5, output current should be same as that of level 0 */
	if (level > 7)	//5
		level = 0;

	for (i = 0; i < level; i++) 
	{
		gpio_set_value_cansleep(gpio_id, GPIO_OUT_HIGH);
		usleep_range(45, 45);
		gpio_set_value_cansleep(gpio_id, GPIO_OUT_LOW);
		usleep_range(45, 45);
	}

	gpio_set_value_cansleep(gpio_id, GPIO_OUT_HIGH);
	usleep_range(800, 800);
	
	return 0;
}

static int32_t qm215_flash_sgm37891_low(
	struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	struct msm_camera_power_ctrl_t *power_info = NULL;
	struct msm_camera_gpio_num_info *gpio_num_info = NULL;

	if (!flash_ctrl) {
		pr_err("device data NULL\n");
		return -EINVAL;
	}

	power_info = &flash_ctrl->power_info;
	gpio_num_info = power_info->gpio_conf->gpio_num_info;

	CDBG("enter 37891 flash qm215_flash_sgm37891_low, now = %d, en = %d, reset = %d\n", 	// 33 torch=34 22
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW], 
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN], 
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_RESET]);

	qm215_flash_sgm37891_cfg(gpio_num_info->gpio_num[SENSOR_GPIO_FL_RESET], 6);
	gpio_set_value_cansleep(gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],GPIO_OUT_HIGH);
	usleep_range(20, 20);
	gpio_set_value_cansleep(gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],GPIO_OUT_LOW);
	usleep_range(45, 45);
	gpio_set_value_cansleep(gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],	GPIO_OUT_HIGH);
	usleep_range(150, 150);

//	CDBG("37891 flash qm215_flash_sgm37891_low Exit\n");
	return 0;
}

static int32_t qm215_flash_sgm37891_high(
	struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	struct msm_camera_power_ctrl_t *power_info = NULL;
	struct msm_camera_gpio_num_info *gpio_num_info = NULL;

	if (!flash_ctrl) {
		pr_err("device data NULL\n");
		return -EINVAL;
	}

	power_info = &flash_ctrl->power_info;
	gpio_num_info = power_info->gpio_conf->gpio_num_info;

	CDBG("enter 37891 flash qm215_flash_sgm37891_high, now = %d, en = %d, reset = %d\n", 
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW], 
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN], 
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_RESET]);

	qm215_flash_sgm37891_cfg(gpio_num_info->gpio_num[SENSOR_GPIO_FL_RESET], 6);
	gpio_set_value_cansleep(gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],GPIO_OUT_HIGH);
	usleep_range(130, 130);

//	CDBG("37891 flash qm215_flash_sgm37891_high Exit\n");
	return 0;
}
#endif

#if 0
static int32_t qm215_flash_low(
	struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	struct msm_camera_power_ctrl_t *power_info = NULL;
	struct msm_camera_gpio_num_info *gpio_num_info = NULL;

	if (!flash_ctrl) {
		pr_err("device data NULL\n");
		return -EINVAL;
	}

	CDBG("Enter");
	power_info = &flash_ctrl->power_info;
	gpio_num_info = power_info->gpio_conf->gpio_num_info;

	if (flash_ctrl->flash_driver_type == FLASH_DRIVER_GPIO &&
		gpio_num_info->valid[SENSOR_GPIO_FL_NOW] &&
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN]) {

		CDBG("flash op low gpio num %d(state:%d) %d(state: %d)\n",
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],
			GPIO_OUT_HIGH,
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],
			GPIO_OUT_HIGH);
		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],
			GPIO_OUT_HIGH);
		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],
			GPIO_OUT_HIGH);
	}
	CDBG("Exit\n");
	return 0;
}

static int32_t qm215_flash_high(
	struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	struct msm_camera_power_ctrl_t *power_info = NULL;
	struct msm_camera_gpio_num_info *gpio_num_info = NULL;

	if (!flash_ctrl) {
		pr_err("device data NULL\n");
		return -EINVAL;
	}

	CDBG("Enter\n");
	power_info = &flash_ctrl->power_info;
	gpio_num_info = power_info->gpio_conf->gpio_num_info;

	if (flash_ctrl->flash_driver_type == FLASH_DRIVER_GPIO &&
		gpio_num_info->valid[SENSOR_GPIO_FL_NOW] &&
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN]) {

		CDBG("flash op high gpio num %d(state:%d) %d(state:%d)\n",
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],
			GPIO_OUT_LOW,
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],
			GPIO_OUT_HIGH);
		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],
			GPIO_OUT_LOW);
		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],
			GPIO_OUT_HIGH);
	}
	CDBG("Exit\n");

	return 0;
}
#endif

static int32_t qm215_flash_release(
	struct msm_flash_ctrl_t *flash_ctrl)
{
	int32_t rc = 0;

	if (!flash_ctrl) {
		pr_err("device data NULL\n");
		return -EINVAL;
	}

	CDBG("Enter qm215_flash_release 37891\n");
	rc = flash_ctrl->func_tbl->camera_flash_off(flash_ctrl, NULL);
	if (rc < 0) {
		pr_err("%s:%d camera_flash_init failed rc = %d",
			__func__, __LINE__, rc);
		return rc;
	}
	flash_ctrl->flash_state = MSM_CAMERA_FLASH_RELEASE;
	CDBG("Exit qm215_flash_release 37891\n");
	return 0;
}

static int32_t qm215_flash_off(struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	struct msm_camera_power_ctrl_t *power_info = NULL;
	struct msm_camera_gpio_num_info *gpio_num_info = NULL;

	if (!flash_ctrl) {
		pr_err("device data NULL\n");
		return -EINVAL;
	}

	CDBG("Enter qm215_flash_off 37891\n");
	power_info = &flash_ctrl->power_info;
	gpio_num_info = power_info->gpio_conf->gpio_num_info;

	if (flash_ctrl->flash_driver_type == FLASH_DRIVER_GPIO &&
		gpio_num_info->valid[SENSOR_GPIO_FL_NOW] &&
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN]) {

		CDBG("flash off gpio num %d(state:%d) %d(state:%d) %d(state: %d)\n",
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_RESET],
			GPIO_OUT_LOW,
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],
			GPIO_OUT_LOW,
		gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],
			GPIO_OUT_LOW);

		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_RESET],
			GPIO_OUT_LOW);		
		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_NOW],
			GPIO_OUT_LOW);
		gpio_set_value_cansleep(
			gpio_num_info->gpio_num[SENSOR_GPIO_FL_EN],
			GPIO_OUT_LOW);
	}

	CDBG("Exit qm215_flash_off 37891\n");
	return 0;
}

static int32_t qm215_flash_gpio_init(
	struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	int32_t rc = 0;

	CDBG("Enter qm215_flash_gpio_init 37891");
	rc = flash_ctrl->func_tbl->camera_flash_off(flash_ctrl, flash_data);

	CDBG("Exit qm215_flash_gpio_init 37891");
	return rc;
}


static int32_t qm215_platform_flash_init(struct msm_flash_ctrl_t *flash_ctrl,
	struct msm_flash_cfg_data_t *flash_data)
{
	CDBG("Enter 37891 qm215_platform_flash_init\n");
	if (!flash_ctrl) {
		pr_err("devices data NULL\n");
		return -EINVAL;
	}

	if (flash_ctrl->flash_driver_type == FLASH_DRIVER_GPIO)
		flash_ctrl->func_tbl = &qm215_gpio_flash_table.func_tbl;

	return 0;
}
static int32_t qm215_flash_platform_probe(struct platform_device *pdev)
{
	int32_t rc = 0;
	struct msm_flash_ctrl_t *flash_ctrl = NULL;
	struct msm_camera_power_ctrl_t *power_info = NULL;
	struct msm_camera_gpio_conf *gpio_conf = NULL;

	if (!pdev->dev.of_node) {
		pr_err("of_node NULL\n");
		return -EINVAL;
	}
	CDBG("enter 37891 flash probe\n");
	rc = camera_flash_platform_probe(pdev);
	if (rc >= 0) {
		flash_ctrl =
			(struct msm_flash_ctrl_t *) platform_get_drvdata(pdev);
		CDBG("device data %pK\n", flash_ctrl);
		if (!flash_ctrl) {
			pr_err("of_node NULL\n");
			return -EINVAL;
		}
		power_info = &flash_ctrl->power_info;
		gpio_conf = power_info->gpio_conf;
		rc = msm_camera_request_gpio_table(gpio_conf->cam_gpio_req_tbl,
				gpio_conf->cam_gpio_req_tbl_size, 1);
		if (rc < 0) {
			pr_err("%s: request gpio failed\n", __func__);
			return rc;
		}
		flash_ctrl->platform_flash_init = qm215_platform_flash_init;
	}
	return rc;
}

static struct platform_driver msm_gpio_flash_platform_driver = {
	.probe = qm215_flash_platform_probe,
	.driver = {
		.name = "qcom,camera-gpio-flash",
		.owner = THIS_MODULE,
		.of_match_table = msm_gpio_flash_dt_match,
	},
};

static int __init qm215_gpio_flash_init_module(void)
{
	int32_t rc = 0;

	CDBG("Enter 37891 flash qm215_gpio_flash_init_module\n");
	rc = platform_driver_register(&msm_gpio_flash_platform_driver);
	if (rc)
		pr_err("platform probe for flash failed");

	return rc;
}

static void __exit qm215_gpio_flash_exit_module(void)
{
	platform_driver_unregister(&msm_gpio_flash_platform_driver);
}

static struct msm_flash_table qm215_gpio_flash_table = {
	.flash_driver_type = FLASH_DRIVER_GPIO,
	.func_tbl = {
		.camera_flash_init = qm215_flash_gpio_init,
		.camera_flash_release = qm215_flash_release,
		.camera_flash_off = qm215_flash_off,
		.camera_flash_low = qm215_flash_sgm37891_low,
		.camera_flash_high = qm215_flash_sgm37891_high,
		.camera_flash_query_current = NULL,
	},
};

module_init(qm215_gpio_flash_init_module);
module_exit(qm215_gpio_flash_exit_module);
MODULE_DESCRIPTION("MSM GPIO FLASH");
MODULE_LICENSE("GPL v2");
