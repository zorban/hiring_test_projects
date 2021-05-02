//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "resource.h"
#include "bioritam.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <stdexcept>

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;                // Buffer to hold vertices
ID3DXMesh*              g_pMeshF = NULL;             // Naslov fizicki
ID3DXMesh*              g_pMeshE = NULL;             // Naslov emotivni
ID3DXMesh*              g_pMeshI = NULL;             // Naslov intelektualni
ID3DXMesh*              g_pMeshDani[ BR_EL ];        // Dani
ID3DXMesh*              g_pMeshCifre[ 10 ];          // Cifre
ID3DXMesh*              g_pMeshMinus;                // Minus
ID3DXMesh*              g_pMeshTacka;                // Tacka
CModelViewerCamera      g_Camera;                    // Kamera
D3DMATERIAL9            g_mtrlS, g_mtrlF;            // Boje
D3DMATERIAL9            g_mtrlE, g_mtrlI, g_mtrlT;
TCHAR                   g_strFont[ LF_FACESIZE ];    // Font name
LPDIRECT3DDEVICE9       g_pd3dDevice;                // Globalni pointer na device
ID3DXFont*              g_pFont = NULL;              // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;        // Sprite for batching draw text calls
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CDXUTDialog             g_SampleUI;                  // dialog for sample specific controls
DXUTDeviceSettings      g_D3DDeviceSettings;         // struktura za device podešenja



struct datum            g_datumRodjendan = { 29, 12, 2006 };
unsigned short          g_satiRodjendan = 0;
unsigned short          g_minutiRodjendan = 0;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_STATIC_DAN          5
#define IDC_STATIC_MJESEC       6
#define IDC_STATIC_GODINA       7
#define IDC_COMBOBOX_DAN        8
#define IDC_COMBOBOX_MJESEC     9
#define IDC_COMBOBOX_GODINA     10
#define IDC_POMOC              11

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;              // The untransformed, 3D position for the vertex
    FLOAT vx, vy, vz;           // The vertex normal vector
} g_Tjemena[ 5 * BR_EL ];       // Tjemena

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL )

// Forward declarations
HRESULT             CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, TCHAR* pstrText, TCHAR* pstrFont,
                                        DWORD dwSize, BOOL bBold, BOOL bItalic );
void                CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void                InitUI();
HRESULT             kreirajTjemena( const datum& datumRodjendan, LPDIRECT3DDEVICE9 pd3dDevice );
HRESULT             InitGeometry( LPDIRECT3DDEVICE9 pd3dDevice );
bool CALLBACK       IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                            bool bWindowed, void* pUserContext );
bool CALLBACK       ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK    OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                        void* pUserContext );
HRESULT CALLBACK    OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                       void* pUserContext );
void CALLBACK       OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK       OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK    MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                             bool* pbNoFurtherProcessing, void* pUserContext );
void CALLBACK       OnD3D9LostDevice( void* pUserContext );
void CALLBACK       OnD3D9DestroyDevice( void* pUserContext );
HRESULT             MaxMultisampling( DXUTDeviceSettings** pDeviceSettings );
void CALLBACK OnD3D9LostDevice( void* pUserContext );

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    InitUI();
    DXUTCreateWindow( L"Bioritam" );
    DXUTCreateDevice( true, 1120, 700 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}

//-----------------------------------------------------------------------------
// Name: CreateD3DXTextMesh()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice,
                            LPD3DXMESH* ppMesh, TCHAR* pstrText,
                            TCHAR* pstrFont, DWORD dwSize,
                            BOOL bBold, BOOL bItalic )
{
    HRESULT hr;
    LPD3DXMESH pMeshNew = NULL;
    HDC hdc = CreateCompatibleDC( NULL );
    if( hdc == NULL )
        return E_OUTOFMEMORY;
    INT nHeight = -MulDiv( dwSize, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
    HFONT hFont;
    HFONT hFontOld;

    hFont = CreateFont( nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                        pstrFont );

    hFontOld = ( HFONT )SelectObject( hdc, hFont );

    hr = D3DXCreateText( pd3dDevice, hdc, pstrText,
                         0.001f, 0.04f, &pMeshNew, NULL, NULL );

    SelectObject( hdc, hFontOld );
    DeleteObject( hFont );
    DeleteDC( hdc );

    if( SUCCEEDED( hr ) )
        *ppMesh = pMeshNew;

    return hr;
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    DXUTComboBoxItem* pItem;
    CDXUTComboBox* pDan;
    WCHAR str[ 5 ];
    void* vpOldItemData;
    UINT i = 1, dana;

    switch( nControlID ) {
    case IDC_COMBOBOX_DAN:
        pItem = ( ( CDXUTComboBox* )pControl )->GetSelectedItem();
        if( pItem )
            g_datumRodjendan.dan = ( UINT )( pItem->pData );
        kreirajTjemena( g_datumRodjendan, g_pd3dDevice );
        break;
    case IDC_COMBOBOX_MJESEC:
        pItem = ( ( CDXUTComboBox* )pControl )->GetSelectedItem();
        if( pItem ) {
            g_datumRodjendan.mjesec = ( UINT )( pItem->pData );
            pDan = g_SampleUI.GetComboBox( IDC_COMBOBOX_DAN );
            vpOldItemData = pDan->GetSelectedData();
            pDan->RemoveAllItems();
            if( PRESTUPNA( g_datumRodjendan.godina ) && g_datumRodjendan.mjesec == 2 )
                dana = 29;
            else
                dana = mjeseci[ g_datumRodjendan.mjesec - 1 ];
		    for( i = 1; i < dana + 1; ++i ) {
			    swprintf_s( str, 5, L"%u", i);
			    pDan->AddItem( str, ( LPVOID )i );
		    }
            if( vpOldItemData < ( LPVOID )i )
                pDan->SetSelectedByData( vpOldItemData );
            g_datumRodjendan.dan = ( UINT) pDan->GetSelectedData();
        }
        kreirajTjemena( g_datumRodjendan, g_pd3dDevice );
        break;
    case IDC_COMBOBOX_GODINA:
        pItem = ( ( CDXUTComboBox* )pControl )->GetSelectedItem();
        if( vpOldItemData ) {
            g_datumRodjendan.godina = ( UINT )( pItem->pData );
            pDan = g_SampleUI.GetComboBox( IDC_COMBOBOX_DAN );
            vpOldItemData = pDan->GetSelectedData();
            pDan->RemoveAllItems();
            if( PRESTUPNA( g_datumRodjendan.godina ) && g_datumRodjendan.mjesec == 2 )
                dana = 29;
            else
                dana = mjeseci[ g_datumRodjendan.mjesec - 1 ];
		    for( i = 1; i < dana + 1; ++i ) {
			    swprintf_s( str, 5, L"%u", i);
			    pDan->AddItem( str, ( LPVOID )i );
		    }
            if( vpOldItemData < ( LPVOID )i )
                pDan->SetSelectedByData( vpOldItemData );
            g_datumRodjendan.dan = ( UINT) pDan->GetSelectedData();
        }
        kreirajTjemena( g_datumRodjendan, g_pd3dDevice );
        break;
    }
//   kreirajTjemena( g_datumRodjendan, g_pd3dDevice );
}

void InitUI() {
    g_SampleUI.Init( &g_DialogResourceManager );
    g_SampleUI.SetCallback( OnGUIEvent );

    g_SampleUI.SetFont( 1, L"Arial", 14, FW_NORMAL );
    g_SampleUI.SetFont( 2, L"Courier New", 16, FW_NORMAL );

    // Static
    g_SampleUI.AddStatic( IDC_STATIC_DAN, L"Дан:", 20, 4, 200, 30 );
	g_SampleUI.GetStatic( IDC_STATIC_DAN )->SetTextColor( D3DCOLOR_ARGB( 255, 130, 130, 130 ) ); // Change color to gray
	g_SampleUI.GetControl( IDC_STATIC_DAN )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
    g_SampleUI.AddStatic( IDC_STATIC_MJESEC, L"Мјесец:", 90, 4, 200, 30 );
	g_SampleUI.GetStatic( IDC_STATIC_MJESEC )->SetTextColor( D3DCOLOR_ARGB( 255, 130, 130, 130 ) ); // Change color to gray
	g_SampleUI.GetControl( IDC_STATIC_MJESEC )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
    g_SampleUI.AddStatic( IDC_STATIC_GODINA, L"Година:", 160, 4, 200, 30 );
	g_SampleUI.GetStatic( IDC_STATIC_GODINA )->SetTextColor( D3DCOLOR_ARGB( 255, 130, 130, 130 ) ); // Change color to gray
	g_SampleUI.GetControl( IDC_STATIC_GODINA )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	g_SampleUI.AddStatic( IDC_POMOC,
                          L"Помјерање модела: Лијеви клик\nПомјерање камере: Десни клик\n"
                          L"Цио екран: Alt-Enter\nЗум: Скрол\nИзлаз: ESC", 810, 4, 300, 600 );
    g_SampleUI.GetStatic( IDC_POMOC )->SetTextColor( D3DCOLOR_ARGB( 255, 70, 70, 70 ) ); // Change color to gray
    g_SampleUI.GetControl( IDC_POMOC )->GetElement( 0 )->dwTextFormat = DT_RIGHT | DT_TOP | DT_WORDBREAK;
    g_SampleUI.GetControl( IDC_POMOC )->GetElement( 0 )->iFont = 1;

	// Combobox
    CDXUTComboBox* pCombo;
	WCHAR str[5];

	g_SampleUI.AddComboBox( IDC_COMBOBOX_GODINA, 160, 20, 70, 24, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 300 );
        datum d;
        danas ( d );
		for( UINT i = d.godina; i > d.godina - 109; --i ) {
			swprintf_s( str, 5, L"%u", i );
			pCombo->AddItem( str, ( LPVOID )i );
		}
        pCombo->SetSelectedByData( ( LPVOID ) g_datumRodjendan.godina );
    }

    g_SampleUI.AddComboBox( IDC_COMBOBOX_MJESEC, 90, 20, 60, 24, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 200 );
		for( UINT i = 1; i < 13; ++i ) {
			swprintf_s( str, 5, L"%u", i);
			pCombo->AddItem( str, ( LPVOID )i );
		}
        pCombo->SetSelectedByData( ( LPVOID ) g_datumRodjendan.mjesec );
    }

    g_SampleUI.AddComboBox( IDC_COMBOBOX_DAN, 20, 20, 60, 24, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 300 );
		for( UINT i = 1; i < mjeseci[ g_datumRodjendan.mjesec - 1 ] + 1; ++i ) {
			swprintf_s( str, 5, L"%u", i);
			pCombo->AddItem( str, ( LPVOID )i );
		}
        pCombo->SetSelectedByData( ( LPVOID ) g_datumRodjendan.dan );
	}
}

//-----------------------------------------------------------------------------
// Kreira sve tačke na ekranu na osnovu zadatog datuma rođendana
//-----------------------------------------------------------------------------
HRESULT kreirajTjemena( const datum& datumRodjendan, LPDIRECT3DDEVICE9 pd3dDevice ) {
   datum xd[ BR_EL ], d;
   bioritam xb[ BR_EL ];
   danas( d );
   try {
       popuniX( xd, xb, d, datumRodjendan, LD );
   }
   catch ( std::domain_error domerr )
   {
       int i = 0;
       while ( domerr.what()[ i ] )
           ++i;
       TCHAR* poruka = new TCHAR[ i + 1 ];
       poruka[ i ] = 0;
       i = 0;
       while( domerr.what()[ i ] ) {
           poruka[ i ] = ( wchar_t ) domerr.what()[ i ];
           ++i;
       }
       MessageBox( DXUTGetHWND(), poruka, L"Bioritam.exe", MB_ICONERROR | MB_OK );
       delete[] poruka;
       return E_FAIL;
   }
   for( int i = 0; i != BR_EL; ++i ) {
      // X koordinate za sva tri grafika su iste
      g_Tjemena[i].x = g_Tjemena[ i + BR_EL ].x
                  = g_Tjemena[ i + 2 * BR_EL ].x
                  = g_Tjemena[ 2 * i + 3 * BR_EL ].x
                  = g_Tjemena[ 2 * i + 1 + 3 * BR_EL ].x
                  = ( ( i - LD ) * D3DX_PI / BR_EL ); 
      // Y koordinate
      g_Tjemena[i].y                       = xb[i].fizicki;
      g_Tjemena[ i + BR_EL ].y             = xb[i].emotivni;
      g_Tjemena[ i + 2 * BR_EL ].y         = xb[i].intelektualni;
      g_Tjemena[ 2 * i + 3 * BR_EL ].y     = -1.0f;
      g_Tjemena[ 2 * i + 1 + 3 * BR_EL ].y = 1.0f;
      // Z koordinate
      g_Tjemena[i].z = g_Tjemena[ i + BR_EL ].z
                     = g_Tjemena[ i + 2 * BR_EL ].z
                     = .0f;
      g_Tjemena[ 2 * i + 3 * BR_EL ].z = g_Tjemena[ 2 * i + 1 + 3 * BR_EL ].z = .0f;
      // Vektori normale svih tjemena su okrenuti ka nama ( u smjeru -z ose )
      g_Tjemena[i].vx = .0f;
      g_Tjemena[i].vy = .0f;
      g_Tjemena[i].vz = -1.0f;
      g_Tjemena[ i + BR_EL ].vx = .0f;
      g_Tjemena[ i + BR_EL ].vx = .0f;
      g_Tjemena[ i + BR_EL ].vx = -1.0f;
      g_Tjemena[ i + 2 * BR_EL ].vx = .0f;
      g_Tjemena[ i + 2 * BR_EL ].vy = .0f;
      g_Tjemena[ i + 2 * BR_EL ].vz = -1.0f;
      g_Tjemena[ 2 * i + 3 * BR_EL ].vx = .0f;
      g_Tjemena[ 2 * i + 3 * BR_EL ].vy = .0f;
      g_Tjemena[ 2 * i + 3 * BR_EL ].vz = -1.0f;
      g_Tjemena[ 2 * i + 1 + 3 * BR_EL ].vx = .0f;
      g_Tjemena[ 2 * i + 1 + 3 * BR_EL ].vy = .0f;
      g_Tjemena[ 2 * i + 1 + 3 * BR_EL ].vz = -1.0f;
   }

    // Fill the vertex buffer.
    VOID* pVertices;
    if( FAILED( g_pVB->Lock( 0, sizeof( g_Tjemena ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, g_Tjemena, sizeof( g_Tjemena ) );
    g_pVB->Unlock();


   // Kreiranje tekst-mesh modelaa (dani)
   wcscpy_s( g_strFont, 32, L"Arial" );
   TCHAR dani[ 3 ];
   ID3DXMesh *pMeshDani[ BR_EL], *tmp;
   for( int i = 0; i != BR_EL; ++i ) {
       swprintf_s( dani, 3, L"%d", xd[i].dan );
       if( FAILED( CreateD3DXTextMesh( pd3dDevice, pMeshDani + i, dani, g_strFont, 0, FALSE, FALSE ) ) )
           return E_FAIL;
       tmp = g_pMeshDani[ i ];
       g_pMeshDani[ i ] = pMeshDani[ i ];
       SAFE_RELEASE( tmp );
   }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Pomoćna funkcija za kreiranje objekata koji koriste d3d device
//-----------------------------------------------------------------------------
HRESULT InitGeometry( LPDIRECT3DDEVICE9 pd3dDevice )
{
    kreirajTjemena( g_datumRodjendan, pd3dDevice );


    // Turn off culling, so we see the front and back of the models
    pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    //pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x80808080 );
    //pd3dDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
    D3DLIGHT9 light;
    D3DXVECTOR3 vecLightDirUnnormalized( -1.0f, -1.0f, 1.0f );
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecLightDirUnnormalized );
    light.Position.x = 1.0f;
    light.Position.y = -1.0f;
    light.Position.z = 1.0f;
    light.Range = 100.0f;
    pd3dDevice->SetLight( 0, &light );
    pd3dDevice->LightEnable( 0, TRUE );

    wcscpy_s( g_strFont, 32, L"Arial" );
    // Cifre
    //TCHAR* cifre = L"01234567890-.";
    TCHAR tmp[2];
    for( int i = 0; i != 10; ++i ) {
        swprintf_s( tmp, 2, L"%d", i );
        if( FAILED( CreateD3DXTextMesh( pd3dDevice, g_pMeshCifre + i, tmp, g_strFont, 0, FALSE, FALSE ) ) )
            return E_FAIL;
    }
    // Miuns i tacka
    SAFE_RELEASE( g_pMeshMinus );
    if( FAILED( CreateD3DXTextMesh( pd3dDevice, &g_pMeshMinus, L"-", g_strFont, 0, FALSE, FALSE ) ) )
        return E_FAIL;
    SAFE_RELEASE( g_pMeshTacka );
    if( FAILED( CreateD3DXTextMesh( pd3dDevice, &g_pMeshTacka, L".", g_strFont, 0, FALSE, FALSE ) ) )
        return E_FAIL;

    // Naslovi
    SAFE_RELEASE( g_pMeshF );
    if( FAILED( CreateD3DXTextMesh( pd3dDevice, &g_pMeshF, L"физички", g_strFont, 0, FALSE, FALSE ) ) )
        return E_FAIL;
    SAFE_RELEASE( g_pMeshE );
    if( FAILED( CreateD3DXTextMesh( pd3dDevice, &g_pMeshE, L"емотивни", g_strFont, 0, FALSE, FALSE ) ) )
        return E_FAIL;
    SAFE_RELEASE( g_pMeshI );
    if( FAILED( CreateD3DXTextMesh( pd3dDevice, &g_pMeshI, L"интелектуални", g_strFont, 0, FALSE, FALSE ) ) )
        return E_FAIL;

    // Definicija materijala
    // Linije - sive
    ZeroMemory( &g_mtrlS, sizeof( D3DMATERIAL9 ) );
    g_mtrlS.Diffuse.r = g_mtrlS.Ambient.r = 0.2f;
    g_mtrlS.Diffuse.g = g_mtrlS.Ambient.g = 0.2f;
    g_mtrlS.Diffuse.b = g_mtrlS.Ambient.b = 0.2f;
    g_mtrlS.Diffuse.a = g_mtrlS.Ambient.a = 1.0f;
    // Fizicki - crveni
    ZeroMemory( &g_mtrlF, sizeof( D3DMATERIAL9 ) );
    g_mtrlF.Diffuse.r = g_mtrlF.Ambient.r = 1.0f;
    g_mtrlF.Diffuse.g = g_mtrlF.Ambient.g = .0f;
    g_mtrlF.Diffuse.b = g_mtrlF.Ambient.b = .0f;
    g_mtrlF.Diffuse.a = g_mtrlF.Ambient.a = 1.0f;
    // Emotivni - zeleni
    ZeroMemory( &g_mtrlE, sizeof( D3DMATERIAL9 ) );
    g_mtrlE.Diffuse.r = g_mtrlE.Ambient.r = .0f;
    g_mtrlE.Diffuse.g = g_mtrlE.Ambient.g = 1.0f;
    g_mtrlE.Diffuse.b = g_mtrlE.Ambient.b = .0f;
    g_mtrlE.Diffuse.a = g_mtrlE.Ambient.a = 1.0f;
    // Intelektualni - plavi
    ZeroMemory( &g_mtrlI, sizeof( D3DMATERIAL9 ) );
    g_mtrlI.Diffuse.r = g_mtrlI.Ambient.r = .0f;
    g_mtrlI.Diffuse.g = g_mtrlI.Ambient.g = .0f;
    g_mtrlI.Diffuse.b = g_mtrlI.Ambient.b = 1.0f;
    g_mtrlI.Diffuse.a = g_mtrlI.Ambient.a = 1.0f;
    // Tekst - bijeli
    ZeroMemory( &g_mtrlT, sizeof( D3DMATERIAL9 ) );
    g_mtrlT.Diffuse.r = g_mtrlT.Ambient.r = .8f;
    g_mtrlT.Diffuse.g = g_mtrlT.Ambient.g = .8f;
    g_mtrlT.Diffuse.b = g_mtrlT.Ambient.b = .8f;
    g_mtrlT.Diffuse.a = g_mtrlT.Ambient.a = 1.0f;

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}

//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{   
    MaxMultisampling( &pDeviceSettings );
    //pDeviceSettings->d3d9.pp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
    //pDeviceSettings->d3d9.pp.MultiSampleQuality = 2;
    return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
    g_pd3dDevice = pd3dDevice;
    HRESULT hr;
    V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye( .2f, 1.3f, -4.4f );
    D3DXVECTOR3 vecAt ( .0f, .0f, .0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );
   return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
    // Create the vertex buffer.
    if( FAILED( pd3dDevice->CreateVertexBuffer( sizeof( g_Tjemena ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }
    if( InitGeometry( pd3dDevice ) == E_FAIL )
       return E_FAIL;
    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 100.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    
    g_SampleUI.GetControl( IDC_POMOC )->SetLocation( pBackBufferSurfaceDesc->Width - 310, 4 );
    
   return S_OK;
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    // Update the camera's position based on user input 
    g_Camera.FrameMove( fElapsedTime );

}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;
    D3DXMATRIXA16 mWorld, mWorld1, mView, mProj;
    //D3DXMATRIXA16 mWorldViewProjection;
    D3DXMATRIXA16 mRot, mTran, mScal, mSet;
    ID3DXMesh* pmeshNaslov = NULL;
    D3DMATERIAL9 mtrlGrafik;
    int buffStart = 0;
    unsigned int sek = 0;
    double bio = 0.0;
    TCHAR tcBioTmp[12];

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        // Get the projection & view matrix from the camera class
        mWorld1 = *g_Camera.GetWorldMatrix();
        mProj = *g_Camera.GetProjMatrix();
        mView = *g_Camera.GetViewMatrix();
        //mWorldViewProjection = mWorld * mView * mProj;
        __timeb64 trenutak; // za crtanje cifri
        trenutak = secMilOd( g_datumRodjendan, g_satiRodjendan, g_minutiRodjendan );

        for ( int j = 0; j != 3; ++j ) {
            // Biramo stranu koju crtamo: 0 - lijevo, 1 - sredina, 2 - desno
            switch ( j ) {
            case 0:
                D3DXMatrixRotationY( &mRot, -7.0f * D3DX_PI / 15 );
                D3DXMatrixTranslation( &mTran, ( 1 + cosf( 7.0f * D3DX_PI / 15 ) ) * -D3DX_PI / 2.0f, .0f, -D3DX_PI / 4.0f );
                mWorld = mRot * mTran * mWorld1;
                pmeshNaslov = g_pMeshF;
                buffStart = 0;
                mtrlGrafik = g_mtrlF;
                sek = trenutak.time % PERF_SEK;
                bio = 100.0 * sin( 2.0 * D3DX_PI *
                      ( ( ( double )sek + ( trenutak.millitm * 1.0 ) / 1000.0 ) / PERF_SEK ) );
                break;
            case 1:
                D3DXMatrixTranslation( &mTran, .0f, .0f, D3DX_PI / 4.0f );
                mWorld = mTran * mWorld1;
                pmeshNaslov = g_pMeshE;
                buffStart = BR_EL;
                mtrlGrafik = g_mtrlE;
                sek = trenutak.time % PERE_SEK;
                bio = 100.0 * sin( 2.0 * D3DX_PI *
                      ( ( ( double )sek + ( trenutak.millitm * 1.0 ) / 1000.0 ) / PERE_SEK ) );
                break;
            case 2:
                D3DXMatrixRotationY( &mRot, 7.0f * D3DX_PI / 15 );
                D3DXMatrixTranslation( &mTran, ( 1 + cosf( 7.0f * D3DX_PI / 15 ) ) * D3DX_PI / 2.0f, .0f, -D3DX_PI / 4.0f );
                mWorld = mRot * mTran * mWorld1;
                pmeshNaslov = g_pMeshI;
                buffStart = 2 * BR_EL;
                mtrlGrafik = g_mtrlI;
                sek = trenutak.time % PERI_SEK;
                bio = 100.0 * sin( 2.0 * D3DX_PI * 
                      ( ( ( double )sek + ( trenutak.millitm * 1.0 ) / 1000.0 ) / PERI_SEK ) );
                break;
            }
            pd3dDevice->SetTransform( D3DTS_WORLD, &mWorld );
            pd3dDevice->SetTransform( D3DTS_VIEW, &mView );
            pd3dDevice->SetTransform( D3DTS_PROJECTION, &mProj );

            // Render the vertex buffer contents
            pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
            // Linije i pravougaonik za danasnji dan
            pd3dDevice->SetMaterial( &g_mtrlS );
            pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 3 * BR_EL, BR_EL );
            pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 3 * BR_EL + 2 * LD, 2 );
            // Crtanje grafika
            pd3dDevice->SetMaterial( &mtrlGrafik );
            pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, buffStart, BR_EL - 1 );
            // Crtanje naslova
            D3DXMatrixScaling( &mScal, .25f, .25f, .25f );
            D3DXMatrixTranslation( &mTran, -D3DX_PI / 7.0, 1.05f, 0 );
            mSet = mScal * mTran * mWorld;
            pd3dDevice->SetTransform ( D3DTS_WORLD, &mSet );
            pmeshNaslov->DrawSubset( 0 );
            // Crtanje cifara
            swprintf_s( tcBioTmp, 11, L"%3.6f", bio );
            D3DXMatrixScaling( &mScal, .25f, .25f, .25f );
            D3DXMatrixRotationX( &mRot, D3DX_PI / 2.0f ); 
            D3DXMatrixTranslation( &mTran, -1.0f, -1.0f, -.5f );
            int i = 0;
            while ( tcBioTmp[ i ] ) {
                D3DXMatrixTranslation( &mTran, -.7f + i * .13f, -1.0f, -.5f );
                mSet = mScal * mRot * mTran * mWorld;
                pd3dDevice->SetTransform( D3DTS_WORLD, &mSet );
                switch( tcBioTmp[i] ) {
                case L'-':
                    g_pMeshMinus->DrawSubset( 0 );
                    break;
                case L'.':
                    g_pMeshTacka->DrawSubset( 0 );
                    break;
                default:
                    g_pMeshCifre[ tcBioTmp[ i ] - L'0' ]->DrawSubset( 0 );
                }
                ++i;
            }
            // Crtanje datuma
            pd3dDevice->SetMaterial( &g_mtrlT );
            for( int i = 0; i != BR_EL; ++i ) {
                    D3DXMatrixTranslation( &mTran, ( i - LD ) * D3DX_PI / BR_EL, 0, 0 );
                    D3DXMatrixScaling( &mScal, 1.0f / 16, 1.0f / 16, 1.0f / 16 );
                    mSet = mScal * mTran * mWorld;
                    pd3dDevice->SetTransform( D3DTS_WORLD, &mSet );
                    g_pMeshDani[i]->DrawSubset( 0 );
            }
            //Renderovanje UI
            V( g_SampleUI.OnRender( fElapsedTime ) );
        }
        V( pd3dDevice->EndScene() );
    }
}

//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
    return 0;
}

//-------------------------------------------------------------------------------------
CD3D9EnumDeviceSettingsCombo* GetCurrentDeviceSettingsCombo( DXUTDeviceSettings*** pDeviceSettings )
{
    CD3D9Enumeration* pD3DEnum = DXUTGetD3D9Enumeration();
    return pD3DEnum->GetDeviceSettingsCombo( (***pDeviceSettings).d3d9.AdapterOrdinal,
                                             (***pDeviceSettings).d3d9.DeviceType,
                                             (***pDeviceSettings).d3d9.AdapterFormat,
                                             (***pDeviceSettings).d3d9.pp.BackBufferFormat,
                                             ( (***pDeviceSettings).d3d9.pp.Windowed == TRUE ) );
}

//-----------------------------------------------------------------------------
// Name: MaxMultisampling()
// Desc: Turn on max multisampling
//-----------------------------------------------------------------------------
HRESULT MaxMultisampling( DXUTDeviceSettings** pDeviceSettings ) {
    g_D3DDeviceSettings = DXUTGetDeviceSettings();
    CD3D9EnumDeviceSettingsCombo* pDeviceSettingsCombo = GetCurrentDeviceSettingsCombo( &pDeviceSettings );
    if( pDeviceSettingsCombo == NULL )
        return E_FAIL;
    int ims = pDeviceSettingsCombo->multiSampleTypeList.GetSize();
    D3DMULTISAMPLE_TYPE msType = pDeviceSettingsCombo->multiSampleTypeList.GetAt( ims - 1 );
    (**pDeviceSettings).d3d9.pp.MultiSampleType = msType;
    DWORD dwMaxQuality = pDeviceSettingsCombo->multiSampleQualityList.GetAt( ims - 1 );
    (**pDeviceSettings).d3d9.pp.MultiSampleQuality = dwMaxQuality - 1;
    
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D9LostDevice();
    for( int i = 0; i != BR_EL; ++i ) {
        if ( i < 10 ) SAFE_RELEASE( g_pMeshCifre[i] );
        SAFE_RELEASE( g_pMeshDani[i] );
    }
    SAFE_RELEASE( g_pVB );
    SAFE_RELEASE( g_pMeshF );
    SAFE_RELEASE( g_pMeshE );
    SAFE_RELEASE( g_pMeshI );
    SAFE_RELEASE( g_pMeshMinus );
    SAFE_RELEASE( g_pMeshTacka );
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D9DestroyDevice();
    for( int i = 0; i != BR_EL; ++i ) {
        if ( i < 10 ) SAFE_RELEASE( g_pMeshCifre[i] );
        SAFE_RELEASE( g_pMeshDani[i] );
    }
    SAFE_RELEASE( g_pVB );
    SAFE_RELEASE( g_pMeshF );
    SAFE_RELEASE( g_pMeshE );
    SAFE_RELEASE( g_pMeshI );
    SAFE_RELEASE( g_pMeshMinus );
    SAFE_RELEASE( g_pMeshTacka );
}
