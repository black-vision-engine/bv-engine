#include "win_sock.h"
// ====================================================================
// Rockey2 ������ API v1.2
// ��������ݱ任����
// ====================================================================

// 1. ���Ҽ���������ӵ� Rockey2 �������豸
extern "C"{ int WINAPI RY2_Find();};
// ����:
// �������ֵС�� 0����ʾ���ص���һ��������룬���庬���뿴����Ĵ�����롣
// �������ֵΪ 0����ʾû���κ� Rockey2 �豸���ڡ�
// �������ֵ���� 0�����ص����ҵ��� Rockey2 �������ĸ�����

// ====================================================================

// 2. ��ָ���ļ�����
EXTERN_C int WINAPI RY2_Open(int mode, DWORD uid, DWORD* hid);
// ����:
// mode �Ǵ򿪼������ķ�ʽ
// mode = 0 ��ʾ���Ǵ򿪵�1���ҵ��� Rockey2������� UID �� HID
// mode > 0 ��ʾ���ǰ� UID ��ʽ�򿪣���ʱ�� mode ��ֵ��ʾ����Ҫ���������
//          Ʃ��: uid=12345678, mode=2 ��ʾ����ϣ���� uid ����12345678 ��
//          ��2�Ѽ�������
// mode = -1 ��ʾ���ǰ� HID ��ʽ�򿪣�Ҫ�� *hid ����Ϊ 0
#define AUTO_MODE	0
#define HID_MODE	-1
// uid(UserID)
// ���� UID ��ʽ�򿪵�ʱ���������Ҫ�򿪼������� UID���� UID �����û�����
// RY2_GenUID ���ܻ�õ��û� ID��
// hid
// ����Ǽ�������Ӳ�� ID������1������/���ֵ������� HID ��ʽ�򿪼�������
// ʱ�򣬱�ʾϣ����Ӳ��ID = *hid �ļ�������
// ���������ַ�ʽ�򿪼��������ڳɹ��򿪼������Ժ�����ⷵ�ؼ�������Ӳ�� ID
// ����ֵ:
// ���ڵ���0		��ʾ�ɹ������صľ��Ǵ򿪵ļ������ľ��
// С��0			���ص���һ��������룬���庬���뿴����Ĵ�����벿�֡�

// ====================================================================

// 3. �ر�ָ���ļ�����
EXTERN_C void WINAPI RY2_Close(int handle);
// ����:
// handle �豸�ľ������ RY2_Open ����ص� handle һ�¡�
// ����:
// �޷�����Ϣ��

// ====================================================================

// 4. �����û� ID
EXTERN_C int WINAPI RY2_GenUID(int handle, DWORD* uid, char* seed, int nReserved);
// ����:
// handle �豸�ľ������ RY2_Open ����ص� handle һ�¡�
// uid ������������ɵ��û� ID �Ӵ˲�������
// seed �û����������������û� ID �����ӣ�����һ����󳤶ȿ����� 64 ���ֽڵ��ַ���
// nReserved ��������Ŀǰ����
#define ROCKEY2_DISABLE_WRITE_PROTECT	0
#define ROCKEY2_ENSABLE_WRITE_PROTECT	1
// ����:
// ���ص���һ��������룬���庬���뿴����Ĵ�����벿�֡�

// ====================================================================

// 5. �������ݱ任
EXTERN_C int WINAPI RY2_Transform(int handle, int len, BYTE* data);
// ����:
// handle �豸�ľ������ RY2_Open ����ص� handle һ�¡�
// len �������� data �ĳ��ȣ���󳤶Ȳ�����32���ֽ�
// data �û����������
// ����:
// data �任�������
// ���ص���һ��������룬���庬���뿴����Ĵ�����벿�֡�

// ====================================================================

// 6. ��ȡ����������
EXTERN_C int WINAPI RY2_Read(int handle, int block_index, char* buffer512);
// ����:
// handle �豸�ľ������ RY2_Open ����ص� handle һ�¡�
// block_index ��������ָ��Ҫ��ȡ������1���飬ȡֵΪ(0-4)
// buffer512 ������Ļ���������Ϊÿ����ĳ��ȹ̶�Ϊ 512 ���ֽڣ��������
// buffer �Ĵ�С������ 512 ���ֽ�
// ����:
// ���ص���һ��������룬���庬���뿴����Ĵ�����벿�֡�

// ====================================================================

// 7. д�����������
EXTERN_C int WINAPI RY2_Write(int handle, int block_index, char* buffer512);
// ����:
// handle �豸�ľ������ RY2_Open ����ص� handle һ�¡�
// block_index ��������ָ��Ҫд�������1���飬ȡֵΪ(0-4)
// buffer512 д���Ļ���������Ϊÿ����ĳ��ȹ̶�Ϊ 512 ���ֽڣ��������
// buffer �Ĵ�С������ 512 ���ֽ�
// ����:
// ���ص���һ��������룬���庬���뿴����Ĵ�����벿�֡�

// ====================================================================

// 8. ���Ӳ���汾��
EXTERN_C int WINAPI RY2_GetVersion(int handle);
// ����:
// handle �豸�ľ������ RY2_Open ����ص� handle һ�¡�
// ����:
// �������0�����ص���Ӳ���İ汾��
// ���С��0�����ص���һ������


// ������� ===========================================================

// �ɹ���û�д���
#define RY2ERR_SUCCESS									0

// û���ҵ�����Ҫ����豸(��������)
#define RY2ERR_NO_SUCH_DEVICE							0xA0100001

// �ڵ��ô˹���ǰ��Ҫ�ȵ��� RY2_Open ���豸(��������)
#define RY2ERR_NOT_OPENED_DEVICE						0xA0100002

// ������ UID ����(��������)
#define RY2ERR_WRONG_UID								0xA0100003

// ��д���������Ŀ���������(��������)
#define RY2ERR_WRONG_INDEX								0xA0100004

// ���� GenUID ���ܵ�ʱ�򣬸����� seed �ַ������ȳ����� 64 ���ֽ�(��������)
#define RY2ERR_TOO_LONG_SEED							0xA0100005

// ��ͼ��д�Ѿ�д������Ӳ��(��������)
#define RY2ERR_WRITE_PROTECT							0xA0100006

// ���豸��(Windows ����)
#define RY2ERR_OPEN_DEVICE								0xA0100007

// ����¼��(Windows ����)
#define RY2ERR_READ_REPORT								0xA0100008

// д��¼��(Windows ����)
#define RY2ERR_WRITE_REPORT								0xA0100009

// �ڲ�����(Windows ����)
#define RY2ERR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL		0xA010000A

// �ڲ�����(Windows ����)
#define RY2ERR_GET_ATTRIBUTES							0xA010000B

// �ڲ�����(Windows ����)
#define RY2ERR_GET_PREPARSED_DATA						0xA010000C

// �ڲ�����(Windows ����)
#define RY2ERR_GETCAPS									0xA010000D

// �ڲ�����(Windows ����)
#define RY2ERR_FREE_PREPARSED_DATA						0xA010000E

// �ڲ�����(Windows ����)
#define RY2ERR_FLUSH_QUEUE								0xA010000F

// �ڲ�����(Windows ����)
#define RY2ERR_SETUP_DI_CLASS_DEVS						0xA0100010

// �ڲ�����(Windows ����)
#define RY2ERR_GET_SERIAL								0xA0100011

// �ڲ�����(Windows ����)
#define RY2ERR_GET_PRODUCT_STRING						0xA0100012

// �ڲ�����
#define RY2ERR_TOO_LONG_DEVICE_DETAIL					0xA0100013

// δ֪���豸(Ӳ������)
#define RY2ERR_UNKNOWN_DEVICE							0xA0100020

// ������֤����(Ӳ������)
#define RY2ERR_VERIFY									0xA0100021

// δ֪����(Ӳ������)
#define RY2ERR_UNKNOWN_ERROR							0xA010FFFF
