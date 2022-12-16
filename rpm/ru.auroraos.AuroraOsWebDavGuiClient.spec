Name:       ru.auroraos.AuroraOsWebDavGuiClient

Summary:    WebDav GUI client for AuroraOS
Version:    0.11
Release:    1
Group:      Qt/Qt
License:    BSD-3-Clause
URL:        https://community.omprussia.ru/open-source
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   qtmozembed-qt5
Requires:   sailfish-components-webview-qt5
Requires:   sailfish-components-webview-qt5-popups
Requires:   sailfish-components-webview-qt5-pickers
BuildRequires:  pkgconfig(auroraapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(qt5embedwidget) >= 1.14.9

%description
The application with the graphical user interface that allows to interact with remote
WebDAV servers. The application allows to interact with the NextCloud, Yandex.Disk
and Communigate servers.

%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude libqwebdav.so.1


%prep
%autosetup

%build

%qmake5 CLIENT_ID=$CLIENT_ID CLIENT_SECRET=$CLIENT_SECRET
%make_build

%install
%make_install


%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png

# >> files
# << files
