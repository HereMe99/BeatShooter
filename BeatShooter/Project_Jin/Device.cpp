#include "stdafx.h"
#include "Device.h"

Device* Device::instance = nullptr;

Device::Device()
{
	device = nullptr;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS param = {};
	param.Windowed = TRUE;
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.AutoDepthStencilFormat = D3DFMT_D16;
	param.EnableAutoDepthStencil = TRUE;

	//vSync(vertical Syncronization) : 수직 동기화

	//vSync끄기
	param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//vSync켜기
	//param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&param,
		&device
	);
}


Device::~Device()
{

}

void Device::BeginRender()
{
	device->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0xff000000, 1.0f, 0);

	device->BeginScene();
}

void Device::EndRender()
{
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

void Device::Release()
{
	d3d->Release();
	device->Release();
	delete instance;
}
