Jak kolejkować E-maile
======================

Jeśli używasz ``SwiftmailerBundle`` do wysyłania e-maili z aplikacji Symfony2, e-maile będą domyślnie wysyłane natychmiast.
Aby uniknąć problemów wydajnościowych w komunikacji pomiędzy ``Swiftmailer`` oraz transporterem e-mail, który może spowodować
że użytkownik będzie musiał czekać na kolejną stronę zanim e-mail zostanie wysłany. Problem oczekiwania na stronę może zostać wyeliminowany 
poprzez wybranie opcji "spool" (kolejkowania) e-maili zamiast natychmiastowego ich wysyłania. Oznacza to że ``Swiftmailer`` nie próbuje 
wysłać e-maila, ale zamiast tego zapisuje je w np. pliku. Inny proces może tak zapisane e-maile odczytać z kolejki oraz je wysłać.
Aktualnie w ``Swiftmailer`` dostępne jest tylko kolejkowanie do pliku.

Aby korzystać z kolejkowania, użyj następującej konfiguracji:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            # ...
            spool:
                type: file
                path: /path/to/spool

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config>
             <swiftmailer:spool
                 type="file"
                 path="/path/to/spool" />
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
             // ...
            'spool' => array(
                'type' => 'file',
                'path' => '/path/to/spool',
            )
        ));

.. tip::

    Jeśli chcesz przechowywać kolejkę gdzieś w katalogu projektu,
    pamiętaj że możesz użyć parametru `%kernel.root_dir%` w odniesieniu do głównego katalogu projektu:

    .. code-block:: yaml

        path: %kernel.root_dir%/spool

Teraz, gdy Twoja aplikacja wysyła e-mail, nie będzie on wysyłany natychmiast, ale dodawany do kolejki.
Wysyłanie wiadomości z kolejki jest robione osobno.
Do wysyłania wiadomości z kolejki wykorzystywana jest specjalna komenda:

.. code-block:: bash

    php app/console swiftmailer:spool:send

Komenda ta posiada opcję do ograniczenia ilości wysłanych wiadomości na raz:

.. code-block:: bash

    php app/console swiftmailer:spool:send --message-limit=10

Możesz także ustawić limit w sekundach:

.. code-block:: bash

    php app/console swiftmailer:spool:send --time-limit=10

Oczywiście nie będziesz uruchamiał tego ręcznie w rzeczywistości.
Zamiast tego komenda ta powinna być dołączona do cron joba lub też innego mechanizmu "zadań zaplanowanych",
oraz odpalana w regularnych odstępach czasu.
