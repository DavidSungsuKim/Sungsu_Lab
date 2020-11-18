
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/uio_driver.h>

static struct uio_info the_uio_info;

static int __init my_probe(struct platform_device *pdev)
{
	int ret_val;
	struct resource *r;
	struct device *dev = &pdev->dev;
	void __iomem *g_ioremap_addr;

	dev_info(dev, "platform_probe enter\n");
	
	/* get our first memory resource from device tree */
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(dev, "IORESOURCE_MEM, 0 does not exist\n");
		return -EINVAL;
	}
	dev_info(dev, "r->start = 0x%08lx\n", (long unsigned int)r->start);
	dev_info(dev, "r->end = 0x%08lx\n", (long unsigned int)r->end);

	/* ioremap our memory region and get virtual address */
	g_ioremap_addr = devm_ioremap(dev, r->start, resource_size(r));
	if (!g_ioremap_addr) {
		dev_err(dev, "ioremap failed \n");
		return -ENOMEM;
	}
	
	/* initialize uio_info struct uio_mem array */
	the_uio_info.name = "led_uio";
	the_uio_info.version = "1.0";
	the_uio_info.mem[0].memtype = UIO_MEM_PHYS;
	the_uio_info.mem[0].addr = r->start; /* physical address needed for the kernel user mapping */
	the_uio_info.mem[0].size = resource_size(r);
	the_uio_info.mem[0].name = "demo_uio_driver_hw_region";
	the_uio_info.mem[0].internal_addr = g_ioremap_addr; /* virtual address for internal driver use */

	/* register the uio device */
	ret_val = uio_register_device(&pdev->dev, &the_uio_info);
	if (ret_val != 0) {
		dev_info(dev, "Could not register device \"led_uio\"...");
	}
	return 0;
}

static int __exit my_remove(struct platform_device *pdev)
{
	uio_unregister_device(&the_uio_info);
	dev_info(&pdev->dev, "platform_remove exit\n");

	return 0;
}

static const struct of_device_id my_of_ids[] = {
	{ .compatible = "arrow,UIO"},
	{},
};

MODULE_DEVICE_TABLE(of, my_of_ids);

static struct platform_driver my_platform_driver = {
	.probe = my_probe,
	.remove = my_remove,
	.driver = {
		.name = "UIO",
		.of_match_table = my_of_ids,
		.owner = THIS_MODULE,
	}
};

module_platform_driver(my_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Liberal <aliberal@arroweurope.com>");
MODULE_DESCRIPTION("This is a UIO platform driver that turns the LED on/off without using system calls");



