根据测试demo来测，样例中使用的不是当前的类MainActivity,而是使用的了其他的类TestAdd。democ测试成功，
成功日志


******************

09-26 10:54:21.710 21371-21371/? D/selfmodify: BEFORE call native method selfmodify() testadd.add(1, 2) 2
09-26 10:54:21.710 21371-21371/? D/selfmodify:  enter the native  method
09-26 10:54:21.710 21371-21371/? D/selfmodify:  startAddress = 7505c000
09-26 10:54:21.710 21371-21371/? D/selfmodify:  endAddress = 7517d000
09-26 10:54:21.710 21371-21371/? D/selfmodify:  search_start_page = 7505c000
09-26 10:54:21.710 21371-21371/? D/selfmodify:  search_start_position = 7505c028
09-26 10:54:21.710 21371-21371/? D/selfmodify: string = dex
                                               035
09-26 10:54:21.710 21371-21371/? D/selfmodify: class_strIdx = 7e1
09-26 10:54:21.710 21371-21371/? D/selfmodify: method_strIdx = d52
09-26 10:54:21.710 21371-21371/? D/selfmodify: class_typeIdx = 38c

09-26 10:54:21.710 21371-21371/? D/selfmodify: class_def_item_address = 750834d8

09-26 10:54:21.710 21371-21371/? D/selfmodify: methodIdx = 1e2c

09-26 10:54:21.710 21371-21371/? D/selfmodify:  classDataOff = 75091c70

09-26 10:54:21.710 21371-21371/? D/selfmodify: staticFieldsSize= 0
09-26 10:54:21.710 21371-21371/? D/selfmodify: staticFieldsSize_addr= 75091c71

09-26 10:54:21.710 21371-21371/? D/selfmodify: instanceFieldsSize= 0
09-26 10:54:21.710 21371-21371/? D/selfmodify: instanceFieldsSize_addr= 75091c72

09-26 10:54:21.710 21371-21371/? D/selfmodify: directMethodsSize= 1
09-26 10:54:21.710 21371-21371/? D/selfmodify: directMethod_addr= 75091c73


09-26 10:54:21.710 21371-21371/? D/selfmodify: virtualMethodsSize= 3
09-26 10:54:21.710 21371-21371/? D/selfmodify: after_skipstaticfield_address = 75091c74

09-26 10:54:21.710 21371-21371/? D/selfmodify: DexMethod_start_address = 75091c74
09-26 10:54:21.710 21371-21371/? D/selfmodify: DexMethod_direct_methodIdx = 1e2b
09-26 10:54:21.710 21371-21371/? D/selfmodify: DexMethod_direct_methodIdx_tmp = 1e2b

09-26 10:54:21.710 21371-21371/? D/selfmodify: DexMethod_virtual_methodIdx = 1e2c
09-26 10:54:21.710 21371-21371/? D/selfmodify: DexMethod_virtual_methodIdx_tmp = 1e2c
09-26 10:54:21.710 21371-21371/? D/selfmodify: codeItem_address = 750c2c74
09-26 10:54:21.710 21371-21371/? D/selfmodify: code_insns_address = 750c2c84
09-26 10:54:21.710 21371-21371/? D/selfmodify: codeinsns_page_address = 750c2000
09-26 10:54:21.710 21371-21371/? D/selfmodify: call native method selfmodify() 1


09-26 10:54:21.810 822-847/? I/ActivityManager: Displayed com.example.selfmodify/.MainActivity: +197ms



***************************************
但是，如果改成当前的
类来做测试，发现失败
***************************************
失败日志
09-26 11:38:07.510 24183-24183/? D/selfmodify: BEFORE call native method selfmodify() testadd.add(1, 2) 2,  ret1() = 1
09-26 11:38:07.510 24183-24183/? D/selfmodify:  enter the native  method
09-26 11:38:07.520 24183-24183/? D/selfmodify:  startAddress = 7505d000
09-26 11:38:07.520 24183-24183/? D/selfmodify:  endAddress = 7517e000
09-26 11:38:07.520 24183-24183/? D/selfmodify:  search_start_page = 7505d000
09-26 11:38:07.520 24183-24183/? D/selfmodify:  search_start_position = 7505d028
09-26 11:38:07.520 24183-24183/? D/selfmodify: string = dex
                                               035
09-26 11:38:07.520 24183-24183/? D/selfmodify:  -------------  found target_string Lcom/example/selfmodify/MainActivity; , index = 2004  
09-26 11:38:07.520 24183-24183/? D/selfmodify: ============  return getStrIdx   index  = 2004 
09-26 11:38:07.520 24183-24183/? D/selfmodify: class_strIdx = 7d4
09-26 11:38:07.520 24183-24183/? D/selfmodify:  -------------  found target_string ret1 , index = 7401  
09-26 11:38:07.520 24183-24183/? D/selfmodify: ============  return getStrIdx   index  = 7401 
09-26 11:38:07.520 24183-24183/? D/selfmodify: method_strIdx = 1ce9
09-26 11:38:07.520 24183-24183/? D/selfmodify: class_typeIdx = 37d
09-26 11:38:07.520 24183-24183/? D/selfmodify: class_def_item_address = 75085bcc
09-26 11:38:07.520 24183-24183/? D/selfmodify: methodIdx = 1e1a
09-26 11:38:07.520 24183-24183/? D/selfmodify:  classDataOff = 750968b3
09-26 11:38:07.520 24183-24183/? D/selfmodify: staticFieldsSize= 0
09-26 11:38:07.520 24183-24183/? D/selfmodify: staticFieldsSize_addr= 750968b4
09-26 11:38:07.520 24183-24183/? D/selfmodify: instanceFieldsSize= 1
09-26 11:38:07.520 24183-24183/? D/selfmodify: instanceFieldsSize_addr= 750968b5
09-26 11:38:07.520 24183-24183/? D/selfmodify: directMethodsSize= 3
09-26 11:38:07.520 24183-24183/? D/selfmodify: directMethod_addr= 750968b6
09-26 11:38:07.520 24183-24183/? D/selfmodify: virtualMethodsSize= 4
09-26 11:38:07.520 24183-24183/? D/selfmodify: after_skipstaticfield_address = 750968b7
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_start_address = 750968ba
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_direct_methodIdx = 1e13
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_direct_methodIdx_tmp = 1e13
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_direct_methodIdx = 1
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_direct_methodIdx_tmp = 1
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_direct_methodIdx = 7
09-26 11:38:07.520 24183-24183/? D/selfmodify: DexMethod_direct_methodIdx_tmp = 7
09-26 11:38:07.520 24183-24183/? D/selfmodify: ****** virtualMethod  1e1a 
09-26 11:38:07.520 24183-24183/? D/selfmodify: ****** DexMethod_methodIdx  1e17 
09-26 11:38:07.520 24183-24183/? D/selfmodify: ****** DexMethod_methodIdx  1 
09-26 11:38:07.520 24183-24183/? D/selfmodify: ****** DexMethod_methodIdx  1 
09-26 11:38:07.520 24183-24183/? D/selfmodify: ****** DexMethod_methodIdx  1 
09-26 11:38:07.520 24183-24183/? D/selfmodify: codeItem_address = 0
09-26 11:38:07.520 24183-24183/? D/selfmodify: code_insns_address = 10
09-26 11:38:07.520 24183-24183/? D/selfmodify: codeinsns_page_address = 0
09-26 11:38:07.520 24183-24183/? A/libc: Fatal signal 11 (SIGSEGV) at 0x00000010 (code=1), thread 24183 (mple.selfmodify)
09-26 11:38:07.620 179-179/? I/DEBUG: pid: 24183, tid: 24183, name: mple.selfmodify  >>> com.example.selfmodify <<<
09-26 11:38:07.680 179-179/? I/DEBUG:     #01  pc 0000171f  /data/app-lib/com.example.selfmodify-2/libselfmodify.so (Java_com_example_selfmodify_MainActivity_selfmodify+506)
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebc68  750968de  /data/dalvik-cache/data@app@com.example.selfmodify-2.apk@classes.dex
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebc6c  7504d4c7  /data/app-lib/com.example.selfmodify-2/libselfmodify.so (getCodeItem+490)
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebc70  7504eb05  /data/app-lib/com.example.selfmodify-2/libselfmodify.so
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebc8c  7505d028  /data/dalvik-cache/data@app@com.example.selfmodify-2.apk@classes.dex
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebc9c  7504eac4  /data/app-lib/com.example.selfmodify-2/libselfmodify.so
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebca4  7504d701  /data/app-lib/com.example.selfmodify-2/libselfmodify.so (Java_com_example_selfmodify_MainActivity_selfmodify+476)
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebcac  7504d723  /data/app-lib/com.example.selfmodify-2/libselfmodify.so (Java_com_example_selfmodify_MainActivity_selfmodify+510)
09-26 11:38:07.680 179-179/? I/DEBUG:          be9ebcb8  75085bcc  /data/dalvik-cache/data@app@com.example.selfmodify-2.apk@classes.dex
09-26 11:38:07.810 822-24198/? W/ActivityManager:   Force finishing activity com.example.selfmodify/.MainActivity

那么，现在需要找到具体原因，为什么加载其他类可以，加载自己的类会出现异常，如果把加载so的过程放到了Application,是否可以
做所有的修改。
--------------------------------------------------------------------
需要验证的即是，
不能自身类不能修改自己的运行时代码，例如这里的MainActivity修改自身的方法提ret1 返回值为2 修改失败。
但是修改TestAdd类的add中的指令修改成功




