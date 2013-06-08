.. index::
   pair: konfiguracja; Framework

Konfiguracja FrameworkBundle
============================

Ten dokument jest nadal w fazie przygotowania. Powinien być dokładny, ale nie 
wszystkie opcje są w pełni opisane.

``FrameworkBundle`` posiada większość z "podstawowych" funkcjonalności frameworka
i może być konfigurowany w konfiguracji aplikacji poprzez klucz ``framework``.
Obejmuje to ustawienia sesji, tłumaczeń, formularzy, walidacji, routingu i wiele więcej.

Konfiguracja
------------

* `secret`_
* `http_method_override`_
* `ide`_
* `test`_
* `trusted_proxies`_
* `form`_
    * enabled
* `csrf_protection`_
    * enabled
    * field_name
* `session`_
    * `name`_
    * `cookie_lifetime`_
    * `cookie_path`_
    * `cookie_domain`_
    * `cookie_secure`_
    * `cookie_httponly`_
    * `gc_divisor`_
    * `gc_probability`_
    * `gc_maxlifetime`_
    * `save_path`_
* `serializer`_
    * :ref:`enabled<serializer.enabled>`
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_
* `profiler`_
    * `collect`_
    * :ref:`enabled<profiler.enabled>`

secret
~~~~~~

**typ**: ``string`` **wymagane**

Jest to łańcuch tekstowy, który powinien być unikalny w skali aplikacji. W praktyce
jest on wykorzystywany do generowania tokenów CSRF, ale też może być używany w innym
kontekście, w którym przydatny jest unikalny ciąg znaków. Jest to parametr
 kontenera usług o nazwie ``kernel.secret``.

.. _configuration-framework-http_method_override:

http_method_override
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    W Symfony 2.3 wprowadzona została opcja ``http_method_override``.

**typ**: ``Boolean`` **domyślnie**: ``true``

Określa czy parametr żądania ``_method`` jest używany jako zamierzona metoda HTTP
dla żądań POST. Jeśli jest włączona, to metoda
:method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>`
jest wywoływana automatycznie. Jest to parametr kontenera usług
o nazwie ``kernel.http_method_override``. Więcej informacji można znaleźć w 
:doc:`/cookbook/routing/method_parameters`.

ide
~~~

**typ**: ``string`` **domyślnie**: ``null``

Jeśli używa się IDE takiego jak TextMate lub Mac Vim, to Symfony może włączyć
do linku komunikatu wyjątku ścieżkę pliku, jaki jest otwierany w IDE.

W przypadku TextMate lub Mac Vim można w prosty sposób skorzystać z wbudowanych wartości

* ``textmate``
* ``macvim``

Można także określić niestandardowy link do pliku. Jeśli się to zrobi, wszystkie
znaki procentu (%) muszą zostać zabezpieczone przez powtórzenie w celu zinterpretowania
tego znaku. Dla przykładu, pełny ciąg znaków dla TextMate będzie wyglądać tak:

.. code-block:: yaml

    framework:
        ide:  "txmt://open?url=file://%%f&line=%%l"

Oczywiście, ponieważ każdy programista używa różnych IDE, lepszym rozwiązaniem
jest ustawienie tej wartości na poziomie systemu. Jest to możliwe poprzez ustawienie
opcji ``xdebug.file_link_format`` w pliku PHP.ini. Jeśli opcja ta jest ustawiona,
nie ma potrzeby definiowania opcji ``ide`` w konfiguracji.


.. _reference-framework-test:

test
~~~~

**typ**: ``Boolean``

Jeśli ten parametr konfiguracyjny znajduje się w konfiguracji (i nie ma wartości
``false``), to będą ładowane usługi związane  z testowaniem aplikacji (np.
``test.client``). Ustawienie to powinno znajdować się w środowisku ``test``
(zazwyczaj poprzez umieszczenie go w ``app/config/config_test.yml``). Więcej
informacji można znaleźć w :doc:`/book/testing`.

trusted_proxies
~~~~~~~~~~~~~~~

**typ**: ``array``

Konfiguruje adresy IP, którymi powinny być zaufane servery proxy. Szczegóły można
znaleźć w :doc:`/components/http_foundation/trusting_proxies`.

.. versionadded:: 2.3
    Została wprowadzona notacja CIDR, więc można stosować białe listy dla całych
    podsieci (np. ``10.0.0.0/8``, ``fc00::/7``).

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        framework:
            trusted_proxies:  [192.0.0.1, 10.0.0.0/8]

    .. code-block:: xml
       :linenos:

        <framework:config trusted-proxies="192.0.0.1, 10.0.0.0/8">
            <!-- ... -->
        </framework>

    .. code-block:: php
       :linenos:

        $container->loadFromExtension('framework', array(
            'trusted_proxies' => array('192.0.0.1', '10.0.0.0/8'),
        ));

.. _reference-framework-form:

form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

name
....

**typ**: ``string`` **domyślnie**: ``null``

Określa nazwę pliku cookie sesji. Domyślnie używane są nazwy plików cookie
określone w ``php.ini`` w dyrektywie ``session.name``.

cookie_lifetime
...............

**typ**: ``integer`` **domyślnie**: ``0``

Określa czas życia sesji w sekundach. Domyślnie używana jest wartość ``0``,
co oznacza, że cookie obowiązuje w czasie sesji przeglądarki.

cookie_path
...........

**typ**: ``string`` **domyślnie**: ``/``

Określa ścieżkę do ustawienia pliku cookie sesji. Domyślnie stosowana jest wartość ``/``.

cookie_domain
.............

**typ**: ``string`` **domyślnie**: ``''``

Określa domenę dla ustawienia pliku cookie. Domyślnie jest pusta, co oznacza, że
stosowana jest nazwa hosta, na którym wygenerowany został plik cookie, zgodnie
ze specyfikacją cookie.

cookie_secure
.............

**typ**: ``Boolean`` **domyślnie**: ``false``

Określa, czy pliki cookie mają być przesyłane przez bezpieczne połączenia.

cookie_httponly
...............

**typ**: ``Boolean`` **domyślnie**: ``false``

Określa, czy pliki cookie maja być dostępne tylko przez protokół HTTP.
Jeśli `true`, to pliki nie będą dostępne dla języków skryptowych, takich jak
JavaScript. Ustawienie to może efektywnie pomóc w zmniejszeniu zagrożeń atakami XSS.

gc_probability
..............

**typ**: ``integer`` **domyślnie**: ``1``

Określa prawdopodobieństwo uruchomienia procesu `garbage collector` (GC) przy
inicjacji każdej sesji. Prawdopodobieństwo jest obliczanie jako
``gc_probability`` / ``gc_divisor``, np. 1/100 oznacza, ze istnieje 1% szansa
uruchomienia procesu GC dla każdego żądania.

gc_divisor
..........

**typ**: ``integer`` **domyślnie**: ``100``

Zobacza `gc_probability`_.

gc_maxlifetime
..............

**typ**: ``integer`` **domyślnie**: ``14400``

Określa liczbę sekund po upływie których dane będą uznawane za "śmieci"
i ewentualnie usunięte. Usuwanie śmieci może nastąpić podczas uruchamiania sesji
i zależy od ustawień `gc_divisor`_ i `gc_probability`_.

save_path
.........

**typ**: ``string`` **domyślnie**: ``%kernel.cache.dir%/sessions``

Określa argument przekazywany do handlera zapisywania. Jeśli wybierze się domyślny
handler plików, to wartością parametru jest ścieżka do miejsca w którym zostały
utworzone pliki. Można również ustawić ten parametr na wartość ``save_path``
z pliku ``php.ini`` przez ustawienie go na ``null``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            session:
                save_path: null

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session save-path="null" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                'save_path' => null,
            ),
        ));

.. _configuration-framework-serializer:

serializer
~~~~~~~~~~

.. _serializer.enabled:

enabled
.......

**typ**: ``boolean`` **domyślnie**: ``false``

Decyduje czy włączyć w kontenerze usług usługę ``serializer`` czy nie.

Więcej szczegółów mozna znaleźć w :doc:`/cookbook/serializer`.

templating
~~~~~~~~~~

assets_base_urls
................

**domyślnie**: ``{ http: [], ssl: [] }``

Opcja umożliwia określenie bazowego adresu URL, która będzie używany w odniesieniu
do aktywów ze stron ``http`` i ``ssl`` (``https``). Zamiast tablicy z jednym elementem
może zostać określona wartość łańcuchowa. Jeśli dostarczanych jest wiele bazowych
adresów URL, Symfony2 wybiera jeden z nich, generując za każdym razem ścieżkę aktywów.

Dla wygody, opcja ``assets_base_urls`` może być ustawiona bezpośrednio z łańcuchem
lub z tablicą, które będą automatycznie organizowane  w kolekcje adresów URL dla żądań
``http`` i ``https``. Jeśli adres URL rozpoczyna się od ``https://`` lub jest jest
`odwołaniem względnym`_ (tj. rozpoczyna się od `//`),  to dodawany będzie do obydwu
kolekcji. Adresy URL rozpoczynajce się od ``http://`` będą dodawane tylko do kolekcji
``http``.

.. _ref-framework-assets-version:

assets_version
..............

**typ**: ``string``

Opcję tą stosuje do globalnego czyszczenia pamięci podręcznej dla zasobów przez
dodanie parametru zapytania do wszystkich renderowanych ścieżek zasobów (np.
``/images/logo.png?v2``). Dotyczy to tylko zasobów renderowanych przez funkcję
``asset`` Twig (lub odpowiednika PHP) a także zasobów aktywowanych z Assetic.

Na przykład załóżmy, że masz następujący kod:

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

Domyślnie będzie to renderować ścieżkę do obrazu, taką jak ``/images/logo.png``.
Teraz aktywujmy opcję ``assets_version``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'], assets_version: v2 }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:templating assets-version="v2">
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            ...,
            'templating'      => array(
                'engines'        => array('twig'),
                'assets_version' => 'v2',
            ),
        ));

Teraz ten samo zasób będzie renderowany jako ``/images/logo.png?v2``. Jeśli używa
się tej funkcjonalności, to **musi się** ręcznie zwiększać wartość ``assets_version``
przed każdym wdrożeniem, tak aby zmienić parametr zapytania.

Można również kontrolować działanie łańcucha zapytań poprzez opcję `assets_version_format`_.

assets_version_format
.....................

**typ**: ``string`` **domyślnie**: ``%%s?%%s``

Określa wzorzec :phpfunction:`sprintf`, który jest stosowany z opcją `assets_version`_
do konstruoawania ścieżek zasobów. Domyślnie wzorzec dodaje wersję zasobu jako
łańcuch zapytania. Na przykład, jeśli ``assets_version_format`` jest ustawiona na
``%%s?version=%%s`` i ``assets_version`` jest ustawiona na ``5``, ścieżką zasobu
będzie ``/images/logo.png?version=5``.

.. note::

    Wszystkie znaki procentu (``%``) w formatowanym łańcuchu muszą być zabezpieczone
    przez podwojenie znaku. Bez tego wartości mogą być źle interpretowane jak
    omówiono to w ":ref:`book-service-container-parameters`".

.. tip::

    Niektóre CDN-y nie obsługują czyszczenia pamięci podręcznej poprzez łańcuch
    zapytań, tak więc wstrzykiwanie wersji do rzeczywistej ścieżki pliku jest
    niezbędne. Na szczęście, ``assets_version_format`` nie jest ograniczona do
    wytwarzania wersjonowanych łańcuchów zapytań.

    Wzorzec ten odbierze ścieżkę oryginalnego zasobu i wersję zgodnie odpowiednio
    z pierwszym i drugim parametrem. Ponieważ ścieżką zasobów jest jeden paramatr,
    nie można zmieniać ścieżki na inną (np. ``/images/logo-v5.png``). Jednak można
    poprzedzić ścieżkę zasobu przedrostkiem stosując wzorzec ``version-%%2$s/%%1$s``,
    co da ścieżkę ``version-5/images/logo.png``.

    Reguły przepisywania adresu URL mogą następnie być wykorzystane do ignorowania
    przedrostka wersji przed zaserwowaniem zasobu. Alternatywnie można skopiować
    zasoby do odpowiedniego katalogu wskazywanego przez ścieżkę wersji na etapie
    procesu wdrażania i zrezygnować z jakiegokolwiek przepisywania adresu URL.
    Ostatnia opcja jest przydatna, jeśli chce aby starsze wersje zasobu pozostawały
    dostępne pod oryginalnym adresem URL.

profiler
~~~~~~~~

.. versionadded:: 2.2
    The ``enabled`` option was added in Symfony 2.2. Previously, the profiler
    could only be disabled by omitting the ``framework.profiler`` configuration
    entirely.

.. _profiler.enabled:

enabled
.......

**domyślnie**: ``true`` w środowisku ``dev`` i ``test``.

Przez ustawienie tego klucza na ``false`` wyłącza się profiler.

.. versionadded:: 2.3

    Opcja ``collect`` jest nowością w Symfony 2.3. Poprzednio, gdy opcja
    ``profiler.enabled`` była ustawiona na ``false``, to profiler był automatycznie
    włączony. Teraz profiler i kolektory mogą być sterowane niezależnie.

collect
.......

**domyślnie**: ``true``

Opcja ta konfiguruje sposób, w jaki zachowuje się profiler, gdy profiler jest włączony.
Jeśli opcja jest ustawiona na ``true``, profiler zbiera dane dla wszystkich żądań.
Jeśli chce się zbierać tylko informacje na żądanie, można ustawić flagę ``collect``
na ``false`` i ręcznie aktywować kolektory danych::

    $profiler->enable();

Pełna domyślna konfiguracja
---------------------------

.. configuration-block::
   :linenos:

    .. code-block:: yaml

        framework:
            secret:               ~
            http_method_override: true
            trusted_proxies:      []
            ide:                  ~
            test:                 ~
            default_locale:       en

            # form configuration
            form:
                enabled:              false
            csrf_protection:
                enabled:              false
                field_name:           _token

            # esi configuration
            esi:
                enabled:              false

            # fragments configuration
            fragments:
                enabled:              false
                path:                 /_fragment

            # profiler configuration
            profiler:
                enabled:              false
                collect:              true
                only_exceptions:      false
                only_master_requests: false
                dsn:                  file:%kernel.cache_dir%/profiler
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~

                    # use the urldecoded format
                    path:                 ~ # Example: ^/path to resource/
                    service:              ~

            # router configuration
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443

                # set to true to throw an exception when a parameter does not match the requirements
                # set to false to disable exceptions when a parameter does not match the requirements (and return null instead)
                # set to null to disable parameter checks against requirements
                # 'true' is the preferred configuration in development mode, while 'false' or 'null' might be preferred in production
                strict_requirements:  true

            # session configuration
            session:
                storage_id:           session.storage.native
                handler_id:           session.handler.native_file
                name:                 ~
                cookie_lifetime:      ~
                cookie_path:          ~
                cookie_domain:        ~
                cookie_secure:        ~
                cookie_httponly:      ~
                gc_divisor:           ~
                gc_probability:       ~
                gc_maxlifetime:       ~
                save_path:            %kernel.cache_dir%/sessions

            # serializer configuration
            serializer:
               enabled: false

            # templating configuration
            templating:
                assets_version:       ~
                assets_version_format:  %%s?%%s
                hinclude_default_template:  ~
                form:
                    resources:

                        # Default:
                        - FrameworkBundle:Form
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Required

                    # Example:
                    - twig
                loaders:              []
                packages:

                    # Prototype
                    name:
                        version:              ~
                        version_format:       %%s?%%s
                        base_urls:
                            http:                 []
                            ssl:                  []

            # translator configuration
            translator:
                enabled:              false
                fallback:             en

            # validation configuration
            validation:
                enabled:              false
                cache:                ~
                enable_annotations:   false
                translation_domain:   validators

            # annotation configuration
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                %kernel.debug%

.. _`protocol-relative`: http://tools.ietf.org/html/rfc3986#section-4.2
