#include <stdbool.h>
#include "sdkconfig.h"

#if CONFIG_RELAY_DRIVER_USE_LP_GPIO
#include <ulp_lp_core_gpio.h>
#endif

#if CONFIG_RELAY_DRIVER_USE_HP_GPIO
#include <hal/gpio_ll.h>
#include <soc/gpio_struct.h>
#include <soc/gpio_reg.h>
#endif

#include <soc/gpio_num.h>

#include "relay_driver.h"

void relay_driver_init(int gpio_num)
{
#if CONFIG_RELAY_DRIVER_USE_LP_GPIO
    ulp_lp_core_gpio_init(gpio_num);
    /* Establish the safe, off level before enabling the output. */
    ulp_lp_core_gpio_set_level(gpio_num, false);
    ulp_lp_core_gpio_output_enable(gpio_num);
    ulp_lp_core_gpio_input_disable(gpio_num);
    ulp_lp_core_gpio_set_output_mode(gpio_num, RTCIO_LL_OUTPUT_NORMAL);
#endif

#if CONFIG_RELAY_DRIVER_USE_HP_GPIO
    gpio_ll_set_level(&GPIO, gpio_num, false);
    gpio_ll_output_enable(&GPIO, gpio_num);
    gpio_ll_pullup_dis(&GPIO, gpio_num);
    gpio_ll_pulldown_dis(&GPIO, gpio_num);
    gpio_ll_input_disable(&GPIO, gpio_num);
    gpio_ll_od_disable(&GPIO, gpio_num);
    gpio_ll_intr_disable(&GPIO, gpio_num);
    gpio_ll_func_sel(&GPIO, gpio_num, PIN_FUNC_GPIO);
#endif
}

void relay_driver_set_power(int gpio_num, bool power)
{
#if CONFIG_RELAY_DRIVER_USE_LP_GPIO
    ulp_lp_core_gpio_set_level(gpio_num, power);
#endif

#if CONFIG_RELAY_DRIVER_USE_HP_GPIO
    gpio_ll_set_level(&GPIO, gpio_num, power);
#endif
}
