.. index::
   single: Emails; Gmail

Jak używać Gmail do wysyłania E-maili
=====================================

W trakcie prac deweloperskich, zamiast używać serwera SMTP do wysyłania maili, możesz użyć Gmail - jest to prostsze i bardziej praktyczne.
Bundle Swiftmailer sprawia że integracja z Gmail jest na prawdę prosta.

.. tip::

    Zamiast używać swojego konta Gmail, oczywistym jest utworzenie specjalnego konta testowego.

W deweloperskim pliku konfiguracyjnym, zmień ustawienie ``transport`` na ``gmail`` oraz ustaw ``username`` oraz ``password``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            transport: gmail
            username:  twoj_uzytkownik_gmail
            password:  twoje_haslo_gmail

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="gmail"
            username="twoj_uzytkownik_gmail"
            password="twoje_haslo_gmail" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'transport' => "gmail",
            'username'  => "twoj_uzytkownik_gmail",
            'password'  => "twoje_haslo_gmail",
        ));

I to wystarczy!

.. note::

    Transport ``gmail`` jest skrótem który tak na prawdę używa transportu ``smtp`` oraz ustawia ``encryption``, 
    ``auth_mode`` oraz ``host`` zgodnie z wymogami Gmail.
