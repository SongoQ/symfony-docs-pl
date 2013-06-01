.. highlight:: php
   :linenothreshold: 2

.. index::
   single: polecenia konsoli; generate:bundle
   single: generowanie pakietu
   
Generowanie szkieletu nowego pakietu
------------------------------------

Stosowanie
~~~~~~~~~~

Polecenie **``generate:bundle``** generuje strukturę nowego pakietu i automatycznie
aktywuje go w aplikacji.

Polecenie domyślnie uruchamiane jest w trybie interaktywnym i zadaje pytania,
aby ustalić nazwę pakietu, lokalizację, format konfiguracyjny i domyślną strukturę:

.. code-block:: bash
   
   $ php app/console generate:bundle
   
Aby wyłączyć tryb interaktywny, trzeba użyć opcji ``--no-interaction``, ale nie
można zapomnieć o przekazaniu potrzebnych opcji:

.. code-block:: bash
   
   $ php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle --no-interaction
   
Dostępne opcje
~~~~~~~~~~~~~~

*  ``--namespace``: Przestrzeń nazw pakietu jaki się tworzy. Nazwa przestrzeni powinna
   się rozpoczynać nazwą "dostawcy" (*ang. vendor*) taką jak nazwa przedsiębiorstwa,
   nazwa projektu lub nazwa klienta, a następnie jedną lub więcej nazw podprzestrzeni
   i kończyć się powinna nazwą samego pakietu (która musi mieć słowo ``Bundle``
   jako przyrostek).
   
   .. code-block:: bash
   
      $ php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle
      
*  ``--bundle-name``: Opcjonalna nazwa pakietu. Musi być to ciąg tekstowy z przyrostkiem *Bundle*:
   
   .. code-block:: bash
      
      $ php app/console generate:bundle --bundle-name=AcmeBlogBundle
      
*  ``--dir``: Katalog w którym przechowywany jest pakiet. Zgodnie z konwencją, polecenie
   wykrywa i wykorzystuje folder ``src/`` aplikacji:
   
   .. code-block:: bash
      
      $ php app/console generate:bundle --dir=/var/www/myproject/src
      
*  ``--format``: (**annotation**) [wartości: yml, xml, php lub annotation] Określa
   format jaki ma być użyty przy generowaniu plików konfiguracyjnych takich jak
   trasowanie. Domyślnie polecenie używa formatu adnotacji. Wybór formatu adnotacji
   wymaga już zainstalowanego pakietu *SensioFrameworkExtraBundle*:
   
   .. code-block:: bash
      
      php app/console generate:bundle --format=annotation
      
*  ``--structure``: (**no**) [wartości: yes|no] Ustala, czy ma zostać wygenerowana
   kompletna domyślna struktura katalogu, w tym pustych folderów publicznych dla dokumentacji,
   aktywów internetowych i słowników tłumaczeń:
   
   .. code-block:: bash
      
      $ php app/console generate:bundle --structure=yes
      
 