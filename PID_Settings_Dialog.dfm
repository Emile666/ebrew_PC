�
 TPID_SETTINGS 0�  TPF0TPID_SettingsPID_SettingsLeft�Top� HelpContext�  BorderStylebsDialogCaptionPID Control SettingsClientHeighthClientWidth� Color	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style OldCreateOrderPositionpoScreenCenterScaledPixelsPerInch`
TextHeight TLabelTS_LabelLeftTopWidth_HeightHintLThe time between two calls to the PID controller (min. 1 sec., max. 20 sec.)Caption&Sample Time TsFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel1LeftTop$WidthkHeightHint+ = Kp, the proportional gain, unity is %/�CCaptionPID &Kc parameterFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel2LeftTop<WidthiHeightHint*= Kc / Ki. Time constant for Integral gainCaptionPID T&i parameterFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel3LeftTopTWidthmHeightHint.= Kd / Kc. Time constant for Differential gainCaptionPID T&d parameterFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabel
Date_LabelLeft� TopWidthHeightCaptionD&ateFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFont  TLabel
Time_LabelLeft� TopWidthHeightCaption&TimeFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFont  TLabelLabel5Left� TopWidth3HeightHintLThe time between two calls to the PID controller (min. 1 sec., max. 20 sec.)CaptionSecondsFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel6Left� Top$WidthHeightHint+ = Kp, the proportional gain, unity is %/�CCaption%/�CFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel7Left� Top<Width3HeightHint*= Kc / Ki. Time constant for Integral gainCaptionSecondsFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel8Left� TopTWidth3HeightHint.= Kd / Kc. Time constant for Differential gainCaptionSecondsFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel4LeftToplWidthlHeightHint4Filter time-constant for lowpass-filtering of D-termCaptionLP-Filter constantFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TLabelLabel9Left� ToplWidth3HeightHint4Filter time-constant for lowpass-filtering of D-termCaptionSecondsFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	  TButtonButton1LeftTopLWidthKHeightCaption&OKDefault	Font.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style ModalResult
ParentFontTabOrder	  TButtonButton2LeftTTopLWidthKHeightCancel	Caption&CancelFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style ModalResult
ParentFontTabOrder
  TButtonButton3Left� TopLWidthKHeightCaption&HelpFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontTabOrderOnClickButton3Click  TEditTS_editLeftxTopWidth1HeightHintLThe time between two calls to the PID controller (min. 1 sec., max. 20 sec.)Font.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	TabOrder   TEditKc_EditLeftxTop Width1HeightHint+ = Kp, the proportional gain, unity is %/�CFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	TabOrder  TEditTi_EditLeftxTop8Width1HeightHint*= Kc / Ki. Time constant for Integral gainFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	TabOrder  TEditTd_EditLeftxTopPWidth1HeightHint.= Kd / Kc. Time constant for Differential gainFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	TabOrder  	TMaskEdit	Date_EditLeft� TopWidth9HeightHint2The date (DD-MM-YY) to activate the PID ControllerEditMask!90/90/00;1;_Font.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 	MaxLength
ParentFontParentShowHintShowHint	TabOrderText  -  -    	TMaskEdit	Time_EditLeft� TopWidth)HeightHint/The time (hh-mm) to activate the PID ControllerEditMask
!90:00;1;_Font.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 	MaxLength
ParentFontParentShowHintShowHint	TabOrderText  :    TRadioGroupRG2LeftTopWidth� Height)HintJIf set to On, the PID controller is switched on at a certain date and timeCaptionTime-switch &ControlColumnsFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style Items.StringsOffOn 
ParentFontParentShowHintShowHint	TabOrderOnClickRG2Click  TRadioGroup	PID_ModelLeftTop� Width� HeightyHint,Select which PID Controller algorithm to useCaption&PID Controller ModelFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style Items.StringsFirst ebrew PID controllerUpdated ebrew PID controllerAllen Bradley Logix 5550 Type AAllen Bradley Logix 5550 Type C 
ParentFontParentShowHintShowHint	TabOrder  TEdit
K_LPF_EditLeftxTophWidth1HeightHint4Filter time-constant for lowpass-filtering of D-termFont.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameComic Sans MS
Font.Style 
ParentFontParentShowHintShowHint	TabOrder  	TCheckBox
CB_PID_dbgLeftTop8Width� HeightCaptionShow PID Controller Debug InfoTabOrder   