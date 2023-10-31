#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "lpa_hal.h"


int cellular_esim_download_profile_with_activationcode(char* ActivationCodeStr, cellular_sim_download_progress_callback download_progress)
{
  /*TODO: Implement Me!*/
  (void)ActivationCodeStr;
  (void)download_progress;
  return (int)0;
}

int cellular_esim_download_profile_from_smds(char* smds)
{
  /*TODO: Implement Me!*/
  (void)smds;
  return (int)0;
}

int cellular_esim_download_profile_from_defaultsmdp(char* smdp)
{
  /*TODO: Implement Me!*/
  (void)smdp;
  return (int)0;
}

int cellular_esim_get_profile_info(eSIMProfileStruct** profile_list, int* nb_profiles)
{
  /*TODO: Implement Me!*/
  (void)profile_list;
  (void)nb_profiles;
  return (int)0;
}

int cellular_esim_enable_profile(char* iccid, int iccid_size)
{
  /*TODO: Implement Me!*/
  (void)iccid;
  (void)iccid_size;
  return (int)0;
}

int cellular_esim_disable_profile(char* iccid, int iccid_size)
{
  /*TODO: Implement Me!*/
  (void)iccid;
  (void)iccid_size;
  return (int)0;
}

int cellular_esim_delete_profile(char* iccid, int iccid_size)
{
  /*TODO: Implement Me!*/
  (void)iccid;
  (void)iccid_size;
  return (int)0;
}

int cellular_esim_lpa_init(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int cellular_esim_lpa_exit(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int cellular_esim_get_eid(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int cellular_esim_get_euicc(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

