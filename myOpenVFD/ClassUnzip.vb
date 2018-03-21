Imports Shell32
Imports System
Imports System.IO
Public Class ClassUnzip
    Private _file As String
    Private _folder As String
    Private _towhere As String
    Private _shell As Shell32.IShellDispatch2
    ''' <summary>Event welches vor dem Entpacken (Unzip) ausgeführt wird.</summary>
    ''' <remarks></remarks>
    Public Event Unzipstart()
    ''' <summary>Event welches nach dem Entpacken (Unzip) ausgeführt wird.</summary>
    ''' <remarks></remarks>
    Public Event UnzipFinishd()
    ''' <summary>Neue Instanz der ClassUnzip Klasse.</summary>
    ''' <param name="file">Datei welche entpackt werden soll.</param>
    ''' <param name="towhere">Zielordner.</param>
    ''' <remarks></remarks>
    Public Sub New(ByVal file As String, ByVal towhere As String)
        _file = file
        _towhere = towhere
        _folder = Path.Combine(Path.GetDirectoryName(_file), _towhere)
        _shell = CreateObject("Shell.Application")
    End Sub
    ''' <summary>Das Entpacken (Unzip) wird ausgeführt.</summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Public Function UnzipNow() As Boolean
        'prüfen ob der ordner existiert
        If Directory.Exists(_folder) = False Then
            Directory.CreateDirectory(_folder)
        End If
        'event für start feuern
        RaiseEvent Unzipstart()
        'entpacken
        Dim temp As Shell32.Folder = _shell.NameSpace((_folder))
        If temp IsNot Nothing Then
            temp.CopyHere(_shell.NameSpace((_file)).Items)
        End If
        'event für ende feuern
        RaiseEvent UnzipFinishd()
        'rückgabe wert setzen
        Return True
    End Function
    ''' <summary>Gibt den Namen der gepackten Datei zurück, oder setzt diesen.</summary>
    ''' <value></value>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Public Property UnZipFile() As String
        Get
            Return _file
        End Get
        Set(ByVal value As String)
            _file = value
        End Set
    End Property
    ''' <summary>Gibt den Zielort (entpacken) zurück, oder setzt diesen.</summary>
    ''' <value></value>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Public Property UnzipTo() As String
        Get
            Return _towhere
        End Get
        Set(ByVal value As String)
            _towhere = value
        End Set
    End Property
End Class