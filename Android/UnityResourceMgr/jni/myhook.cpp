
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>
#include "CydiaSubstrate.h"
#include "myhook.h"
//#include <jni.h>
//#include <dlfcn.h>
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,"sdgsec",__VA_ARGS__)



extern "C" void* mono_image_open_from_data_with_name(char *, unsigned int , int , void *, int , const char *);

void* (*old_mono_image_open_from_data_with_name)(char *, unsigned int , int , void *, int , const char *);

// ��д·��
static char* m_WritePath = NULL;

char* jstringTostring(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

// ��ʼ����дĿ¼
void Java_com_UnityResources_Test_SendWritePath(JNIEnv* env, jstring path)
{
	if (!m_WritePath)
	{
		m_WritePath = jstringTostring(env, path);
	}
}

void *
my_mono_image_open_from_data_with_name (char *data, unsigned int data_len, int need_copy, void *status, int refonly, const char *name)
{
	//MonoCLIImageInfo *iinfo;
	//MonoImage *image;
	//char *datac;
//���������libhack.so/hack.cpp�е�HackMonoDll��������ʱֻ��һ���ӡû���������ݣ���hook���֮�������������޸�so���й�����չ
	//HackMonoDll(&data,&data_len,name);

//ִ��HackMonoDll���������ִ��ԭ������

	// �ҵ�·��
	InitWritePath();

	return old_mono_image_open_from_data_with_name(data, data_len, need_copy, status, refonly, name);
}







int HookMonoFuc()
{


	MSHookFunction((void*)&mono_image_open_from_data_with_name,
			        			(void*)&my_mono_image_open_from_data_with_name,
			        			(void **)&old_mono_image_open_from_data_with_name);

}
