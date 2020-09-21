/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.30
* Profile  : STM32
* Platform : STM.STM32.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_ApplicationApplication.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreView.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "Application.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000222, /* ratio 55.68 % */
  0xB8004D00, 0x80090452, 0x24D80032, 0x0006F004, 0x010C8188, 0x064859C8, 0x0A000B00,
  0xA8441800, 0x87C36150, 0x01B40045, 0x8020E620, 0x5611158A, 0x4005787C, 0x007A001A,
  0x12789184, 0x38000840, 0x422C0CA1, 0x971A0005, 0x0438E600, 0xCCE1F42A, 0x8E5F4287,
  0xE110B384, 0x92002AC3, 0x94C99D0C, 0x6397C2A9, 0x03A00264, 0x8B1BA1B4, 0xF5640118,
  0xCBEB55BA, 0x9C0167AF, 0xE1B42A19, 0x1165A748, 0xD0FB29E2, 0x7784500E, 0xF0054E82,
  0x87D6A575, 0x7C46D51E, 0x5AEB54A8, 0x02702E84, 0x4833B400, 0xB92F8ACD, 0xB33B24BE,
  0x603C8023, 0xDAE50E8D, 0x9F588EB0, 0xECCE7F43, 0xC3E694DB, 0x22B30889, 0x6117697C,
  0x3F9DB2D1, 0xB0887CFF, 0x42B2DF35, 0xD87DA633, 0xD34A6270, 0x156687B0, 0x66F0FB7D,
  0x369970D0, 0xF2765A4C, 0xCC00DE6D, 0x3709FF1C, 0x8986DAF0, 0x02546633, 0xAE50B3C0,
  0x9777390F, 0x786C943D, 0xA33F6CE4, 0xB135F936, 0x7619C57B, 0x200F9DE6, 0x2591C231,
  0xDFD4D655, 0x97DE4002, 0x5BE4CD31, 0x6629FB7D, 0x00004060, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 800, 480 }};
static const XPoint _Const0001 = { 0, 0 };
static const XPoint _Const0002 = { 800, 0 };
static const XPoint _Const0003 = { 800, 480 };
static const XPoint _Const0004 = { 0, 480 };
static const XColor _Const0005 = { 0x56, 0x56, 0x56, 0xFF };
static const XRect _Const0006 = {{ 614, 40 }, { 758, 184 }};
static const XRect _Const0007 = {{ 40, 30 }, { 660, 150 }};
static const XStringRes _Const0008 = { _StringsDefault0, 0x0002 };
static const XColor _Const0009 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XRect _Const000A = {{ 40, 170 }, { 760, 260 }};
static const XStringRes _Const000B = { _StringsDefault0, 0x0028 };
static const XRect _Const000C = {{ 40, 280 }, { 780, 400 }};
static const XStringRes _Const000D = { _StringsDefault0, 0x0078 };
static const XRect _Const000E = {{ 160, 430 }, { 620, 465 }};
static const XStringRes _Const000F = { _StringsDefault0, 0x00F8 };

/* Initializer for the class 'Application::Application' */
void ApplicationApplication__Init( ApplicationApplication _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRoot__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ApplicationApplication );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text1, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text2, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text3, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ApplicationApplication );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const0001 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const0002 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const0003 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const0004 );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0000 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, _Const0005 );
  CoreRectView__OnSetBounds( &_this->Image, _Const0006 );
  ViewsImage_OnSetAutoSize( &_this->Image, 1 );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0007 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertTop );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0008 ));
  ViewsText_OnSetColor( &_this->Caption, _Const0009 );
  CoreRectView__OnSetBounds( &_this->Text1, _Const000A );
  ViewsText_OnSetWrapText( &_this->Text1, 1 );
  ViewsText_OnSetAlignment( &_this->Text1, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text1, EwLoadString( &_Const000B ));
  ViewsText_OnSetColor( &_this->Text1, _Const0009 );
  CoreRectView__OnSetBounds( &_this->Text2, _Const000C );
  ViewsText_OnSetWrapText( &_this->Text2, 1 );
  ViewsText_OnSetAlignment( &_this->Text2, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text2, EwLoadString( &_Const000D ));
  ViewsText_OnSetColor( &_this->Text2, _Const0009 );
  CoreRectView__OnSetBounds( &_this->Text3, _Const000E );
  ViewsText_OnSetWrapText( &_this->Text3, 1 );
  ViewsText_OnSetAlignment( &_this->Text3, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text3, EwLoadString( &_Const000F ));
  ViewsText_OnSetColor( &_this->Text3, _Const0009 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Caption ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text3 ), 0 );
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, ApplicationApplication_OnTouch );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, ApplicationApplication_OnTouch );
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &ApplicationLogo, ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &ApplicationFontLarge, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text1, EwLoadResource( &ApplicationFontSmall, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text2, EwLoadResource( &ApplicationFontSmall, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text3, EwLoadResource( &ApplicationFontSmall, ResourcesFont ));
}

/* Re-Initializer for the class 'Application::Application' */
void ApplicationApplication__ReInit( ApplicationApplication _this )
{
  /* At first re-initialize the super class ... */
  CoreRoot__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsImage__ReInit( &_this->Image );
  ViewsText__ReInit( &_this->Caption );
  ViewsText__ReInit( &_this->Text1 );
  ViewsText__ReInit( &_this->Text2 );
  ViewsText__ReInit( &_this->Text3 );
}

/* Finalizer method for the class 'Application::Application' */
void ApplicationApplication__Done( ApplicationApplication _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRoot );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsImage__Done( &_this->Image );
  ViewsText__Done( &_this->Caption );
  ViewsText__Done( &_this->Text1 );
  ViewsText__Done( &_this->Text2 );
  ViewsText__Done( &_this->Text3 );

  /* Don't forget to deinitialize the super class ... */
  CoreRoot__Done( &_this->_Super );
}

/* This is a slot method connected with the touch handler. Each time the user touches 
   on the screen, this method is called. As a result, the position of the logo image 
   will be changed. */
void ApplicationApplication_OnTouch( ApplicationApplication _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreRectView__OnSetBounds( &_this->Image, EwSetRectOrigin( _this->Image.Super1.Bounds, 
  EwMovePointNeg( _this->SimpleTouchHandler.CurrentPos, EwNewPoint( EwGetRectW( 
  _this->Image.Super1.Bounds ) / 2, EwGetRectH( _this->Image.Super1.Bounds ) / 2 ))));
}

/* Variants derived from the class : 'Application::Application' */
EW_DEFINE_CLASS_VARIANTS( ApplicationApplication )
EW_END_OF_CLASS_VARIANTS( ApplicationApplication )

/* Virtual Method Table (VMT) for the class : 'Application::Application' */
EW_DEFINE_CLASS( ApplicationApplication, CoreRoot, SimpleTouchHandler, SimpleTouchHandler, 
                 SimpleTouchHandler, SimpleTouchHandler, _None, _None, "Application::Application" )
  CoreRectView_initLayoutContext,
  CoreRoot_GetRoot,
  CoreRoot_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreRoot_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreRoot_OnSetFocus,
  CoreRoot_DispatchEvent,
  CoreRoot_BroadcastEvent,
  CoreRoot_InvalidateArea,
EW_END_OF_CLASS( ApplicationApplication )

/* Include a file containing the font resource : 'Application::FontLarge' */
#include "_ApplicationFontLarge.h"

/* Table with links to derived variants of the font resource : 'Application::FontLarge' */
EW_RES_WITHOUT_VARIANTS( ApplicationFontLarge )

/* Include a file containing the bitmap resource : 'Application::Logo' */
#include "_ApplicationLogo.h"

/* Table with links to derived variants of the bitmap resource : 'Application::Logo' */
EW_RES_WITHOUT_VARIANTS( ApplicationLogo )

/* Include a file containing the font resource : 'Application::FontSmall' */
#include "_ApplicationFontSmall.h"

/* Table with links to derived variants of the font resource : 'Application::FontSmall' */
EW_RES_WITHOUT_VARIANTS( ApplicationFontSmall )

/* Embedded Wizard */
