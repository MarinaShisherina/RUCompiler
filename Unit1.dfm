object Form1: TForm1
  Left = 318
  Top = 173
  Width = 720
  Height = 475
  Caption = #1052#1042#1057'-17/'#1064#1080#1096#1077#1088#1080#1085#1072' '#1052#1072#1088#1080#1085#1072'/'#1042#1072#1088#1080#1072#1085#1090'-18'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 2
    Width = 265
    Height = 351
    Caption = #1050#1086#1076' '#1085#1072' '#1103#1079#1099#1082#1077' RU'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object RichEdit1: TRichEdit
      Left = 2
      Top = 15
      Width = 261
      Height = 334
      Align = alClient
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -15
      Font.Name = 'Arial Narrow'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
    end
  end
  object GroupBox2: TGroupBox
    Left = 280
    Top = 2
    Width = 417
    Height = 351
    Caption = #1050#1086#1076' '#1085#1072' '#1103#1079#1099#1082#1077' C'
    TabOrder = 1
    object RichEdit2: TRichEdit
      Left = 2
      Top = 15
      Width = 413
      Height = 334
      Align = alClient
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -15
      Font.Name = 'Arial Narrow'
      Font.Style = []
      Lines.Strings = (
        '')
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
    end
  end
  object Edit1: TEdit
    Left = 8
    Top = 360
    Width = 689
    Height = 25
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 386
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N2: TMenuItem
        Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N4Click
      end
      object N12: TMenuItem
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100
        OnClick = N12Click
      end
    end
    object N5: TMenuItem
      Caption = #1055#1088#1080#1084#1077#1088#1099
      object N6: TMenuItem
        Caption = #1062#1077#1083#1099#1077' '#1095#1080#1089#1083#1072
        OnClick = N6Click
      end
      object N7: TMenuItem
        Caption = #1052#1072#1090#1088#1080#1094#1099
        OnClick = N7Click
      end
      object while1: TMenuItem
        Caption = #1062#1080#1082#1083' while'
        OnClick = while1Click
      end
      object N8: TMenuItem
        Caption = #1059#1089#1083#1086#1074#1085#1099#1081' '#1086#1087#1077#1088#1072#1090#1086#1088
        OnClick = N8Click
      end
    end
    object N9: TMenuItem
      Caption = #1058#1088#1072#1085#1089#1083#1080#1088#1086#1074#1072#1090#1100
      OnClick = N9Click
    end
    object N10: TMenuItem
      Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100
      OnClick = N10Click
    end
    object N11: TMenuItem
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1086#1096#1080#1073#1082#1091
      OnClick = N11Click
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 40
    Top = 386
  end
  object OpenDialog1: TOpenDialog
    Left = 72
    Top = 386
  end
end
