#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/bitops.h>
#include <linux/types.h>
#include <linux/delay.h>

#define NT50358A_REG_AVDD		0x00
#define NT50358A_REG_AVEE		0x01
#define NT50358A_REG_ENABLE		0x03
#define NT50358A_REG_MTP		0xFF

static struct regmap_config nt50358a_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

static int nt50358a_regulator_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct regmap *regmap;
	int buffer[3] = {0};

        printk("nt50358a (I2C BUS) driver prboe...\n");
        if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
        	pr_err("I2C not supported");
        	return -ENODEV;
    	}

	regmap = devm_regmap_init_i2c(client, &nt50358a_regmap_config);
	if (IS_ERR(regmap)) {
		pr_err("init regmap failed for nt50358a, rc = %ld\n",
						PTR_ERR(regmap));
		return PTR_ERR(regmap);
	}

	regmap_read(regmap, NT50358A_REG_AVDD, buffer);
	printk("========= nt50358a avdd = 0x%x =========\n", buffer[0]);

	if (0x0F != buffer[0])
	{
		regmap_write(regmap, NT50358A_REG_AVDD, 0x0F); //5.5v
		mdelay(1);
		regmap_write(regmap, NT50358A_REG_AVEE, 0x0F); //5.5v
		mdelay(1);
		regmap_write(regmap, NT50358A_REG_MTP, 0x80);
		mdelay(1);

		regmap_read(regmap, NT50358A_REG_AVDD, buffer);
		mdelay(1);
		regmap_read(regmap, NT50358A_REG_AVEE, buffer + 1);
		mdelay(1);
		regmap_read(regmap, NT50358A_REG_MTP, buffer + 2);

		printk("======== nt50358a avdd = 0x%x, avee = 0x%x, mtp = 0x%x ========\n", buffer[0], buffer[1], buffer[2]);
	}

	return 0;
}

static int nt50358a_regulator_remove(struct i2c_client *client)
{
	return 0;
}

static struct of_device_id nt50358a_match_table[] = {
	{ .compatible = "nt,nt50358a", },
	{},
};
MODULE_DEVICE_TABLE(of, nt50358a_match_table);

static const struct i2c_device_id nt50358a_id[] = {
	{"nt50358a", -1},
	{ },
};

static int nt50358a_suspend(struct device *dev)
{
	return 0;
}

static int nt50358a_resume(struct device *dev)
{
	return 0;
}

static const struct dev_pm_ops nt50358a_pm_ops = {
	.resume = nt50358a_resume,
	.suspend = nt50358a_suspend,
};

static struct i2c_driver nt50358a_regulator_driver = {
	.driver = {
		.name		= "nt50358a",
		.owner		= THIS_MODULE,
		.of_match_table	= nt50358a_match_table,
		.pm		= &nt50358a_pm_ops,
	},
	.probe = nt50358a_regulator_probe,
	.remove = nt50358a_regulator_remove,
	.id_table = nt50358a_id,
};

static int __init nt50358a_init(void)
{
	return i2c_add_driver(&nt50358a_regulator_driver);
}
subsys_initcall(nt50358a_init);

static void __exit nt50358a_exit(void)
{
	i2c_del_driver(&nt50358a_regulator_driver);
}
module_exit(nt50358a_exit);

MODULE_DESCRIPTION("TI NT50358A regulator driver");
MODULE_LICENSE("GPL v2");