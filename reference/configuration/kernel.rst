.. index::
    pair: konfiguracja; Kernel

Konfigurowanie kernela (np. AppKernel)
======================================

W klasie samego kernela można dokonać pewnych zmian  konfiguracyjnych (tj. w pliku ``app/AppKernel.php``).
Można to zrobić nadpisując określone metody klasy :class:`Symfony\\Component\\HttpKernel\\Kernel`.

Konfiguracja
------------

* `Kernel Name`_
* `Root Directory`_
* `Cache Directory`_
* `Log Directory`_

Kernel Name
~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``app`` (tj. nazwa katalogu w którym znajduje się kernel )

Aby zmienić to ustawienie, należy nadpisać metodę
:method:`Symfony\\Component\\HttpKernel\\Kernel::getName`. Alternatywnie można
przenieść swój kernel do innego katalogu. Na przykład, jeśli przeniesie się kernel
do katalogu ``foo`` (wewnątrz ``app``), nazwą kernela będzie ``foo``.

Nazwa kernela nie jest zazwyczaj bezpośrednio istotna – jest używana w generowaniu
plików pamięci podręcznej. Jeśli ma się aplikację z wieloma kernelami, to najprostszym
sposobem na to, aby każdy z nich posiadał unikalną nazwę, jest powielenie katalogu
``app``i przemianowanie go na coś innego (np. ``foo``).

Root Directory
~~~~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: katalog ``AppKernel``

Zwraca katalog główny kernela. Jeżeli używa się Symfony Standard
Edition, to katalog główny odnosi się do katalogu ``app``.

Aby zmienić to ustawienie, należy nadpisać metodę
:method:`Symfony\\Component\\HttpKernel\\Kernel::getRootDir`::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getRootDir()
        {
            return realpath(parent::getRootDir().'/../');
        }
    }

Cache Directory
~~~~~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``$this->rootDir/cache/$this->environment``

Zwraca ścieżkę do katalogu pamięci podręcznej. Aby zmienić to ustawienie, należy
nadpisać metodę
:method:`Symfony\\Component\\HttpKernel\\Kernel::getCacheDir`. W celu uzyskania
więcej informacji proszę przeczytać ":ref:`override-cache-dir`".

Log Directory
~~~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``$this->rootDir/logs``

Zwraca ścieżkę do katalogu dzienników. Aby zmienić to ustawienie, należy nadpisać
metodę :method:`Symfony\\Component\\HttpKernel\\Kernel::getLogDir`.
W celu uzyskania więcej informacji proszę przeczytać ":ref:`override-logs-dir`".
