<!-- SPDX-FileCopyrightText: 2026 Roman Koch -->
<!-- SPDX-License-Identifier: MIT -->

# Arbeitsregeln

Sprich mit Roman auf Deutsch.

## Entwicklung und Integration

- Beginne Featurebranches auf dem aktuellen `develop`. `develop` ist der Integrationsbranch. Übernimm abgeschlossene Featurebranches mit `git merge --ff-only`. Übernimm freigegebene Stände aus `develop` ebenfalls per Fast-Forward nach `main`.
- Entwickle Verhaltensänderungen nach Red–Green–Refactor. Reproduziere bei einem Fehler zuerst das Problem mit einem fehlschlagenden Test. Bei Build- und Paketierungsänderungen prüfe den betroffenen Build- oder Installationspfad vor und nach der Änderung.
- Prüfe die betroffenen Fälle lokal vor der Integration. Gib bei der Übergabe an, was auf dem Host, im Crossbuild und am echten Gerät geprüft wurde.
- Jede neue oder angepasste Datei erhält eine SPDX-Kopfzeile in ihrer Kommentarsyntax. Bewahre die tatsächlichen Urheber und die Lizenz von Fremdcode.
- Halte die Keypad-Repositories eigenständig. Geschützte PQ-Quellen und Zugangsdaten bleiben außerhalb dieser Repositories und ihrer CI.

## Linux-Host und Yocto

Dieses Repository enthält die USB-Hostbibliothek, die CLI `wizard` und den Yocto-Layer `meta-varikey`. Bibliothek und CLI müssen auch ohne angeschlossene Hardware baubar und installierbar sein.

```sh
cmake -S . -B build-host -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build-host
ctest --test-dir build-host --output-on-failure --no-tests=error
cmake --install build-host --prefix "$PWD/build-host/stage"
```

Bei Änderungen an Bibliothek oder Installationsregeln prüfe einen separaten CMake-Verbraucher mit `find_package(VariKeyHost CONFIG REQUIRED)` und `VariKey::host`. Für die Paketprüfung aktiviere `meta-varikey` in einer Scarthgap-Buildumgebung und führe `bitbake variwizard` aus. Prüfe das Zielpaket für die CLI, die Laufzeitbibliothek und das Entwicklungspaket. PQ-Edge-Image und Blackboard-Prozess werden gesondert integriert.
