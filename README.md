# Podstawy-informatyki-projekt
1.Sklonuj repozytorium i przejdź do katalogu:
git clone https://github.com/Rosepose95/Podstawy-informatyki-projekt
cd Podstawy-informatyki-projekt
2. Pobieranie biblioteki SFML
Projekt wymaga biblioteki SFML w wersji 3.0.2 
Pobierz wersję 64-bit stąd: https://www.sfml-dev.org/download/sfml/3.0.2/
Rozpakuj archiwum na dysku.
3. Konfiguracja w Visual Studio
Otwórz MojaGra.vcxproj. Upewnij się, że na górze masz ustawione x64.
Wejdź we właściwości projektu 
C/C++ -> General -> Additional Include Directories: wskaż folder include z SFML.
C/C++ -> Language -> C++ Language Standard: ustaw na ISO C++17 Standard (/std:c++17).
Linker -> General -> Additional Library Directories: wskaż folder lib z SFML.
4. Linkowanie bibliotek (Input)
Wejdź w Linker -> Input -> Additional Dependencies 
Dla wersji Debug (sfml-graphics-d.lib, sfml-window-d.lib, sfml-system-d.lib, sfml-audio-d.lib, sfml-network-d.lib)
Dla wersji Release (sfml-graphics.lib, sfml-window.lib, sfml-system.lib, sfml-audio.lib, sfml-network.lib
5. Obsługa plików DLL 
Aby gra się uruchomiła, pliki  bibliotek muszą znajdować się w tym samym folderze co plik .exe.
Skopiuj wszystkie pliki z rozszerzeniem .dll z folderu SFML/bin do folderu wyjściowego projektu:
Dla wersji Debug: skopiuj pliki z końcówką -d.dll (np. sfml-graphics-d-3.dll).
Dla wersji Release: skopiuj pliki bez końcówki -d (np. sfml-graphics-3.dll).
6. Kompilacja i uruchomienie
Naciśnij F5 
