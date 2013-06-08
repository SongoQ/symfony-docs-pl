.. index::
   double: konfiguracja; Swiftmailer
   

Konfiguracja SwiftmailerBundle
==============================

Ten dokument informacyjny jest jeszcze w trakcie tworzenia. Powinien być dokładny,
ale wszystkie opcje nie są jeszcze całkowicie opisane. Pełna lista opcji konfiguracyjnych
podana jest w rozdziale `Full Default Configuration`_.

Klucz ``swiftmailer`` konfiguruje integrację Symfony z Swiftmailer, który jest
odpowiedzialny za tworzenie i wysyłanie wiadomości e-mail.

Konfiguracja
------------

* `transport`_
* `username`_
* `password`_
* `host`_
* `port`_
* `encryption`_
* `auth_mode`_
* `spool`_
    * `type`_
    * `path`_
* `sender_address`_
* `antiflood`_
    * `threshold`_
    * `sleep`_
* `delivery_address`_
* `disable_delivery`_
* `logging`_

transport
~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``smtp``

Jawne określenie metody wysyłania wiadomości e-mail.
Prawidłowymi wartościami są:

* smtp
* gmail (zobacz :doc:`/cookbook/email/gmail`)
* mail
* sendmail
* null (tak samo jak ustawienie `disable_delivery`_  na ``true``)

username
~~~~~~~~

**typ**: ``string``

Nazwa użytkownika, gdy używany jest protokół ``smtp``.

password
~~~~~~~~

**typ**: ``string``

Hasło, gdy używany jest protokół ``smtp``.

host
~~~~

**typ**: ``string`` **domyślnie**: ``localhost``

Nazwa hosta do połączenia w protokole ``smtp``.

port
~~~~

**typ**: ``string`` **domyślnie**: 25 lub 465 (w zależności od `encryption`_)

Port gdy używa się protokołu ``smtp``. Domyślną wartością jest 465 gdy przy
połączeniu szyfrowanym i 25 przy połączeniu nieszyfrowanym.

encryption
~~~~~~~~~~

**typ**: ``string``

Tryb szyfrowania przy uzywaniu protokołu ``smtp`` jako metodzie transportu.
Prawidłowe wartości, to ``tls``, ``ssl`` lub ``null`` (co oznacza brak szyfrowania).

auth_mode
~~~~~~~~~

**typ**: ``string``

Tryb szyfrowania przy używaniu protokołu ``smtp`` jako metodzie transportu.
Prawidłowe wartości, to ``tls``, ``ssl`` lub ``null`` (co oznacza brak szyfrowania).

spool
~~~~~

Szczegółowe informacje o buforowaniu wiadomości e-mail, zobacz :doc:`/cookbook/email/spool`.

type
....

**typ**: ``string`` **domyślnie**: ``file``

Metoda używana dla przechowywania buforowanych wiadomości. Obecnie obsługiwana
jest tylko wartość ``file``. Jednak indywidualne buforowanie powinno być możliwe
poprzez utworzenie usługi o nazwie ``swiftmailer.spool.myspool`` i ustawienie
wartości tej opcji na ``myspool``.

path
....

**typ**: ``string`` **domyślnie**: ``%kernel.cache_dir%/swiftmailer/spool``

Gdy używana jest szpula ``file``, jest to ścieżka wskazująca, gdzie buforowane
wiadomości są przechowywane.

sender_address
~~~~~~~~~~~~~~

**typ**: ``string``

Jeśli ustawiona, to wszystkie wiadomości będą dostarczane z tym adresem jako adresem
zwrotnym, na który powinny być dostarczane wiadomości zwrotne. Jest to obsługiwane
wewnętrznie przez klasę ``Swift_Plugins_ImpersonatePlugin`` Swiftmailer.

antiflood
~~~~~~~~~

threshold
.........

**typ**: ``string`` **domyślnie**: ``99``

Używane w ``Swift_Plugins_AntiFloodPlugin``. Jest to liczba wiadomości e-mail
do wysłania przed restartem transportu.

sleep
.....

**typ**: ``string`` **domyślnie**: ``0``

Używane w ``Swift_Plugins_AntiFloodPlugin``. Jest to liczba sekund „drzemki”
podczas restartu transportu.

delivery_address
~~~~~~~~~~~~~~~~

**typ**: ``string``

Jeśli ustawiono, wszystkie wiadomości są wysyłane na ten adres a nie na adresy
rzeczywistych odbiorców. Jest to przydatne podczas programowania. Na przykład,
ustawiając to w pliku ``config_dev.yml`` można być pewnym, że wszystkie wiadomości
zostaną wysłane na to jedno konto.

Używa tego ``Swift_Plugins_RedirectingPlugin``. Rzeczywiste adresy odbiorców są
dostępne w nagłówkach ``X-Swift-To``, ``X-Swift-Cc`` i ``X-Swift-Bcc``.

disable_delivery
~~~~~~~~~~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``false``

Jeśli ``true``, wartość ``transport`` będzie automatycznie ustawiany na ``null``
i żadna wiadomość nie będzie w rzeczywistości dostarczana.

logging
~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``%kernel.debug%``

Jeśli ``true``, to dla Swiftmailer jest aktywowany kolektor danych a informacje
są dostępne w profilerze.

Pełna domyśłna konfiguracja
---------------------------

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        swiftmailer:
            transport:            smtp
            username:             ~
            password:             ~
            host:                 localhost
            port:                 false
            encryption:           ~
            auth_mode:            ~
            spool:
                type:                 file
                path:                 "%kernel.cache_dir%/swiftmailer/spool"
            sender_address:       ~
            antiflood:
                threshold:            99
                sleep:                0
            delivery_address:     ~
            disable_delivery:     ~
            logging:              "%kernel.debug%"

    .. code-block:: xml
       :linenos:

        <swiftmailer:config
            transport="smtp"
            username=""
            password=""
            host="localhost"
            port="false"
            encryption=""
            auth_mode=""
            sender_address=""
            delivery_address=""
            disable_delivery=""
            logging="%kernel.debug%"
        >
            <swiftmailer:spool
                path="%kernel.cache_dir%/swiftmailer/spool"
                type="file"
            />

            <swiftmailer:antiflood
                sleep="0"
                threshold="99"
            />
        </swiftmailer:config>
