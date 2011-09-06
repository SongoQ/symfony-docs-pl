Jak pracować z E-mailami podczas prac rozwojowych
=================================================

Kiedy tworzysz aplikację wysyłającą e-maile, raczej nie będziesz chciał ich 
wysyłać do odbiorców w czasie prac rozwojowych.
Jeśli w Symfony2 używasz ``SwiftmailerBundle``, to możesz to łatwo 
osiągnąć poprzez ustawienia konfiguracyjne bez konieczności wprowadzania zmian w kodzie.
Istnieją dwie głównie wykorzystywane opcje, jeśli chodzi o obsługę e-maili podczas rozwoju:
(a) wyłączenie wysyłania e-maili w ogóle lub (b) wysyłanie wszystkich e-maili na określony adres.

Wyłączenie wysyłania
--------------------

Możesz wyłączyć wysyłanie e-maili poprzez ustawienie opcji ``disable_delivery`` na ``true``.
Jest to domyślna wartośc w środowisku ``test`` w Standardowej dystrybucji.
Jeśli ustawisz tę opcję w pliku konfiguracyjnym ``test`` e-maile nie będą wysyłane podczas wywoływania testów,
ale będą wysyłane w środowisku ``prod`` oraz ``dev``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        swiftmailer:
            disable_delivery:  true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            disable-delivery="true" />

    .. code-block:: php

        // app/config/config_test.php
        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery'  => "true",
        ));

Jeśli chcesz także wyłączyć dostarczanie e-maili w środowisku ``dev``, 
po prostu ustaw to w pliku ``config_dev.yml``.

Wysyłanie na Określony Adres
----------------------------

Możesz także wybrać że wszystkie e-maile będą wysyłane na określony adres,
zamiast adresu faktycznie podanego podczas wysyłki.
Możesz to zrobić przy użyciu opcji ``delivery_address``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            delivery_address:  dev@example.com

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            delivery-address="dev@example.com" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'delivery_address'  => "dev@example.com",
        ));

Teraz wyobraź sobie że wysyłasz e-mail na adres ``recipient@example.com``.

.. code-block:: php

    public function indexAction($name)
    {
        $message = \Swift_Message::newInstance()
            ->setSubject('Hello Email')
            ->setFrom('send@example.com')
            ->setTo('recipient@example.com')
            ->setBody($this->renderView('HelloBundle:Hello:email.txt.twig', array('name' => $name)))
        ;
        $this->get('mailer')->send($message);

        return $this->render(...);
    }

W środowisku ``dev``, e-mail zostanie wysłany na adres ``dev@example.com``.
Swiftmailer doda także dodatkowy nagłówek do e-maila, ``X-Swift-To`` zawierający zamieniony adres,
dzięki czemu będziesz mógł nadal sprawdzić do kogo e-mail miał zostać dostarczony.

.. note::

    Oprócz adresu ``to``, opcja ta zaprzestanie wysyłania e-maili do ustawionych adresów
    ``CC`` oraz ``BCC``. Swiftmailer doda dodatkowe nagłówki do e-maili z nadpisanymi adresami.
    Są to ``X-Swift-Cc`` oraz ``X-Swift-Bcc`` dla wiadomości ``CC`` oraz ``BCC``.

Podgląd z Web Debug Toolbar
---------------------------

Możesz zobaczyć każdy z wysłanych e-maili na stronie jeśli jesteś w środowisku ``dev``
z użytym Web Debug Toolbar.
Ikona e-mail na pasku narzędzi informuje ile e-maili zostało wysłanych. Jeśli ją klikniesz, zobaczysz
raport z dokładniejszymi informacjami.

Jeśli po wysłaniu e-maila natychmiast robisz przekierowanie, musisz ustawić opcję ``intercept_redirects``
na ``true`` w pliku ``config_dev.yml`` dzięki czemu zobaczysz e-maile w Web Debug Toolbar przed przekierowaniem.
