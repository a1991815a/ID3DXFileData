#ifndef __GAPPLICATION__
#define __GAPPLICATION__
#include "GameHeader.h"

class GWinProcessor{

};

typedef bool(GWinProcessor::* process_t)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

struct GWINPROCESS{
	GWinProcessor*		instance;
	process_t			callback;
};

#define GPROC_CALLBACK(FUNC) (process_t)(&FUNC)

class GApplication{
	friend LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
	friend GApplication* dxGetApp();
public:
	GApplication(const GString& name, int x, int y, int cx, int cy);
	~GApplication();

	const GString& getWindowName() const{
		return m_strName;
	}
	const Size& getWindowSize() const{
		return m_ClientSize;
	};
	HWND getWindow() const{
		return m_hWnd;
	}
	int Run();
	void DestroyWindow();
	void Exit();

	void RegisterProcess(void* instance, process_t func);
	void UnregisterProcess(void* instance);
private:
	HWND							m_hWnd;						//��ǰ������
	GString							m_strName;					//������
	Size							m_ClientSize;				//�ͻ�����С
	std::vector<GWINPROCESS>		m_WinProcessVector;			//windows��Ϣ����ص��б�



	static GApplication* s_pApplication;	//��ǰӦ��
};

GApplication* dxGetApp();
#endif