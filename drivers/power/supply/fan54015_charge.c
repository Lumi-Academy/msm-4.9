/**********uniscope-driver-modify-file-on-qualcomm-platform*****************/
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/param.h>
#include <linux/stat.h>
#include <linux/reboot.h>
#include <linux/string.h>
#include "fan54015.h"
#define UNISCOPE_DRIVER_L580
#if (defined UNISCOPE_DRIVER_L560S) || (defined UNISCOPE_DRIVER_L580)//add L580
#include <linux/power_supply.h>//zhangbing add
#endif

#define SPRDCHG__DEBUG
#ifdef SPRDCHG__DEBUG
#define SPRDCHG_54015_DEBUG(format, arg...) printk( "sprd 54015: " "@@@" format, ## arg)
#else
#define SPRDCHG_54015_DEBUG(format, arg...)
#endif

void sprdchg_fan54015_set_vindpm(int vin)
{
	
	BYTE reg_val = 0x0;
	SPRDCHG_54015_DEBUG("sprdchg_fan54015_set_vindpm vin =%d\n",vin);
	if(vin <= 4213){
		reg_val = 0x0;
	}else if (vin >= 4773){
		reg_val = 0x7;
	}else{
		reg_val = (vin -4213) / 80 +1;
	}
	fan54015_set_vindpm(reg_val);
}
void  sprdchg_fan54015_termina_cur_set(int cur)
{
	BYTE reg_val = 0x0;
	SPRDCHG_54015_DEBUG("sprdchg_fan54015_termina_cur_set cur =%d\n",cur);
	if(cur <= 49){
		reg_val = 0x0;
	}else if( cur >= 388){
		reg_val = 0x7;
	}else{
		reg_val = (cur -48) /48;
	}
	fan54015_termina_cur_set(reg_val);
}
void sprdchg_fan54015_termina_vol_set(int vol)
{
	BYTE reg_val = 0x0;
	SPRDCHG_54015_DEBUG("sprdchg_fan54015_termina_vol_set vol =%d\n",vol);
	if(vol <= 3500){
		reg_val = 0x0;
	}else if( vol >= 4440){
		reg_val = 0x2f; /* from 0x2e to 0x2f according spec by jianaiping 2016-6-22 */
	}else{
		reg_val = (vol -3499)/20;
	}
	fan54015_termina_vol_set(reg_val);
}
void sprdchg_fan54015_set_safety_vol(int vol)
{
	BYTE reg_val = 0x0;
	if(vol < 4200 )
		vol = 4200;
	if(vol > 4440)
		vol = 4440;
	reg_val = (vol - 4200) / 20 + 1;
	fan54015_set_safety_vol(reg_val);
}
void sprdchg_fan54015_set_safety_cur(int cur)
{
	BYTE reg_val = 0x7;
	fan54015_set_safety_cur(reg_val);
}

BYTE sprdchg_fan54015_cur2reg(int cur)
{
	BYTE reg_val;
	if(cur < 650)
		reg_val = 0;
	if((cur >= 650) && (cur < 750))
		reg_val = 1;
	if((cur >= 750) && (cur < 850))
		reg_val = 2;
	if((cur >= 850) && (cur < 1050))
		reg_val = 3;
	if((cur >= 1050) && (cur <1150))
		reg_val = 4;
	if((cur >= 1150) && (cur < 1350))
		reg_val = 5;
	if((cur >= 1350) && (cur < 1450))
		reg_val = 6;
	if(cur >= 1450)
		reg_val = 7;
	return reg_val;
}
int sprdchg_fan54015_reg2cur(BYTE reg_val)
{
	int cur = 0;
	switch (reg_val){
		case 0:
			cur = 550;
			break;
		case 1:
			cur = 650;
			break;
		case 2:
			cur = 750;
			break;
		case 3:
			cur = 850;
			break;
		case 4 :
			cur = 1050;
			break;
		case 5:
			cur = 1150;
			break;
		case 6:
			cur = 1350;
			break;
		case 7:
			cur = 1450;
			break;
		default :
			cur = 550;
	}
	return cur;
}
int sprdchg_tq24157_reg2cur(BYTE reg_val)
{
	return (550 + reg_val * 100);
}
BYTE sprdchg_tq24157_cur2reg(int cur)
{
	BYTE reg_val;
	SPRDCHG_54015_DEBUG("sprdchg_tq24157_cur2reg\n");
	if(cur < 650){
		reg_val = 0;
	}else if(cur >= 1250){
		reg_val = 7;
	}else{
		reg_val = (cur -550)/100;
	}
	return reg_val;
}
void  sprdchg_fan54015_reset_timer(void)
{
	fan54015_reset_timer();
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_reset_timer);
void sprdchg_fan54015_otg_enable(int enable)
{
	fan54015_otg_enable(enable);
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_otg_enable);

void sprdchg_fan54015_stop_charging(void)
{
	fan54015_stop_charging();
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_stop_charging);

int sprdchg_fan54015_get_charge_status(void)
{
	BYTE chg_status = 0;
	chg_status = fan54015_get_chg_status();
	switch(chg_status){
		case CHG_READY:
			SPRDCHG_54015_DEBUG("fan54015 charge ready\n");
			return POWER_SUPPLY_STATUS_NOT_CHARGING;
		case CHG_CHGING:
			SPRDCHG_54015_DEBUG("fan54015 is charging\n");
			return POWER_SUPPLY_STATUS_CHARGING;
		case CHG_DONE:
			SPRDCHG_54015_DEBUG("fan54015 charge full\n");
			return POWER_SUPPLY_STATUS_FULL;
		default:
			SPRDCHG_54015_DEBUG("fan54015 charge fault\n");
			return POWER_SUPPLY_STATUS_DISCHARGING;
	}
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_get_charge_status);

int sprdchg_fan54015_get_charge_vbus_valid(void)
{
	BYTE monitor_val = 1;
	monitor_val = fan54015_get_monitor_val();
	return monitor_val;
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_get_charge_vbus_valid);

int sprdchg_fan54015_get_charge_fault(void)
{
	BYTE fault_val = 0,ret = 0;
	fault_val = fan54015_get_fault_val();
	switch(fault_val){
		case CHG_NO_FAULT:
			SPRDCHG_54015_DEBUG("no fault\n");
			break;
		case CHG_THM_SHUTDOWN:
			SPRDCHG_54015_DEBUG("chg hot\n");
			break;
		case CHG_VBAT_OVP:
			SPRDCHG_54015_DEBUG("chg vbat ovp\n");
			break;
		default:
			SPRDCHG_54015_DEBUG("chg unspec fault\n");
	}
	return ret;
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_get_charge_fault);

 void sprdchg_fan54015_start_chg(int cur)
{
	
	BYTE reg_val = 0,vendor_id = VENDOR_FAN54015;
	SPRDCHG_54015_DEBUG("sprdchg_fan54015_start_chg cur =%d\n",cur);
	
	if(vendor_id == VENDOR_FAN54015){
		reg_val = sprdchg_fan54015_cur2reg(cur);
	}
	if(vendor_id == VENDOR_TQ24157){
		reg_val = sprdchg_tq24157_cur2reg(cur);
	}
	
	SPRDCHG_54015_DEBUG("sprdchg_fan54015_start_chg,reg_val=%d\n",reg_val);
	fan54015_set_chg_current(reg_val);
	fan54015_enable_chg();
	
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_start_chg);

 int sprdchg_fan54015_get_chgcur(void)
{
	BYTE reg_val = 0, vendor_id = VENDOR_FAN54015;
	int cur = 550;
	reg_val = fan54015_get_chg_current();
	vendor_id = fan54015_get_vendor_id();

	if(vendor_id == VENDOR_FAN54015){
		cur = sprdchg_fan54015_reg2cur(reg_val);
	}
	if(vendor_id == VENDOR_TQ24157){
		cur = sprdchg_tq24157_reg2cur(reg_val);
	}
	SPRDCHG_54015_DEBUG("sprdchg_fan54015_get_chgcur cur =%d\n",cur);
	return cur;
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_get_chgcur);
void sprdchg_fan54015_adjust_cur(int adjust)
{
	BYTE reg_val = fan54015_get_chg_current();
	if(adjust > 0){
		if(reg_val == 7)
			return;
		reg_val++;
	}else{
		if(!reg_val)
			return;
		reg_val--;
	}
	fan54015_set_chg_current(reg_val);
	fan54015_enable_chg();
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_adjust_cur);
int sprdchg_fan54015_get_chgcur_step(int cur, int increase)
{
	BYTE reg_val = fan54015_get_chg_current(),vendor_id = VENDOR_FAN54015;
	int new_cur = 0;
	if(increase > 0){
		if(reg_val == 7)
			return 0;
		reg_val++;
	}else{
		if(!reg_val)
			return 0;
		reg_val--;
	}
	vendor_id = fan54015_get_vendor_id();

	if(vendor_id == VENDOR_FAN54015){
		new_cur = sprdchg_fan54015_reg2cur(reg_val);
	}
	if(vendor_id == VENDOR_TQ24157){
		new_cur = sprdchg_tq24157_reg2cur(reg_val);
	}
	
	if(increase > 0){
		return (new_cur -cur);
	}else{
		return (cur-new_cur);
	}
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_get_chgcur_step);

void sprdchg_fan54015_init(void)
{
	sprdchg_fan54015_set_safety_vol(4350);
	sprdchg_fan54015_set_safety_cur(1000);
	fan54015_init();
	sprdchg_fan54015_set_vindpm(4350);
	if(1){
		fan54015_enable_cur_terminal(1);
		sprdchg_fan54015_termina_cur_set(100);
	}else{
		fan54015_enable_cur_terminal(0);
		
	}
	sprdchg_fan54015_termina_vol_set(4300);
}
EXPORT_SYMBOL_GPL(sprdchg_fan54015_init);


struct sprd_ext_ic_operations sprd_extic_op ={
	.ic_init = sprdchg_fan54015_init,
	.charge_start_ext = sprdchg_fan54015_start_chg,
	.charge_stop_ext = sprdchg_fan54015_stop_charging,
	.get_charging_status = sprdchg_fan54015_get_charge_status,
	.get_charging_fault =  sprdchg_fan54015_get_charge_fault,
	.get_charging_vbus_valid = sprdchg_fan54015_get_charge_vbus_valid,
	.timer_callback_ext = sprdchg_fan54015_reset_timer,
	.otg_charge_ext = sprdchg_fan54015_otg_enable,
	.get_charge_cur_ext = sprdchg_fan54015_get_chgcur,
	.set_termina_cur_ext = sprdchg_fan54015_termina_cur_set,
	.set_termina_vol_ext = sprdchg_fan54015_termina_vol_set,
	.get_chgcur_step = sprdchg_fan54015_get_chgcur_step,
};


const struct sprd_ext_ic_operations *sprd_get_ext_ic_ops(void){
	return &sprd_extic_op;
}

