Name:       ru.auroraos.AuroraOsWebDavGuiClient
Summary:    The application with the graphical user interface that allows to interact with remote
WebDAV servers. The application allows to interact with the NextCloud, Yandex.Disk
and Communigate servers.
Version:    0.1
Release:    1
License:    BSD-3-Clause
URL:        https://auroraos.ru
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(auroraapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)


%description
The application with the graphical user interface that allows to interact with remote
WebDAV servers. The application allows to interact with the NextCloud, Yandex.Disk
and Communigate servers.

%prep
%autosetup

%build
%qmake5
%make_build

%install
%qmake5_install

%files
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
