
### 1. aquestalkライブラリのダウンロード

https://www.a-quest.com/download.html#a-etc

### 2. ライブラリのインストール

```
cd /Users/riki/Downloads/aqtkpico_esp32
mv libaquestalk.a /Users/riki/Documents/Arduino/hardware/espressif/esp32/tools/sdk/lib/
mkdir /Users/riki/Documents/Arduino/hardware/espressif/esp32/tools/sdk/include/aquestalk
mv aquestalk.h /Users/riki/Documents/Arduino/hardware/espressif/esp32/tools/sdk/include/aquestalk/
cd /Users/riki/Documents/Arduino/hardware/espressif/esp32/
cat platform.txt
nano platform.local.txt
```

```platform.local.txt
compiler.c.extra_flags="-I{compiler.sdk.path}/include/aquestalk"
compiler.cpp.extra_flags="-I{compiler.sdk.path}/include/aquestalk"
compiler.c.elf.libs=-lgcc -lopenssl -lbtdm_app -lfatfs -lwps -lcoexist -lwear_levelling -lhal -lnewlib -ldriver -lbootloader_support -lpp -lmesh -lsmartconfig -ljsmn -lwpa -lethernet -lphy -lapp_trace -lconsole -lulp -lwpa_supplicant -lfreertos -lbt -lmicro-ecc -lcxx -lxtensa-debug-module -lmdns -lvfs -lsoc -lcore -lsdmmc -lcoap -ltcpip_adapter -lc_nano -lrtc -lspi_flash -lwpa2 -lesp32 -lapp_update -lnghttp -lspiffs -lespnow -lnvs_flash -lesp_adc_cal -llog -lexpat -lm -lc -lheap -lmbedtls -llwip -lnet80211 -lpthread -ljson  -lstdc++  -laquestalk
```

### 参考

http://blog-yama.a-quest.com/?eid=970188
