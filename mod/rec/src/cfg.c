#include <stdio.h>
#include <unistd.h>
#include "cfg.h"

gsf_rec_parm_t rec_parm = {
  .rch = {
    [0] = {0,},
  },
};

int jscon_parm_load(char *filename, gsf_rec_parm_t *cfg)
{
  if(access(filename, 0)) return -1;
  FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
	char *data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
  cJSON* json = cJSON_Parse(data);
  free(data);
  if (json)
	{
    sjb_bind_gsf_rec_parm_t(json, 1, cfg, 0, 0);
    cJSON_Delete(json);
	}
  return 0;
}

int jscon_parm_save(char *filename, gsf_rec_parm_t *cfg)
{
  FILE *f=fopen(filename,"wb");
  cJSON* out = cJSON_CreateObject();
  sjb_bind_gsf_rec_parm_t(out, 0, cfg, 0, 0);
  char* print = cJSON_Print(out);
  if(print)
  {
	  fprintf(f, "%s", print);
	  fclose(f);
	  free(print);
  }
  cJSON_Delete(out);
  return 0;
}