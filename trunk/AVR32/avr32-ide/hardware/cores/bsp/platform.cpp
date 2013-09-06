
#include <avr32.h>

extern "C"
{
	void _bsp_init(void);
	int main(void);
}

static scif_gclk_opt_t gc_dfllif_ref_opt = { SCIF_GCCTRL_SLOWCLOCK, 0, false };
static pcl_freq_param_t pcl_dfll_freq_param = {
  PCL_MC_DFLL0,
  TARGET_MCUCLK_FREQ_HZ,
  TARGET_PBACLK_FREQ_HZ,
  TARGET_PBACLK_FREQ_HZ,
  0,
  0,
  0,
  TARGET_DFLL_FREQ_HZ,
  &gc_dfllif_ref_opt
};

void _bsp_init(void)
{
	pcl_configure_clocks(&pcl_dfll_freq_param);
	
	main();
}

