.. index::
   single: Emails

Jak wysyłać E-maile
=================

Wysyłanie e-maili jest jednym z podstawowych zadań dla aplikacji webowych,
a zarazem miejscem gdzie możemy spodziewać się komplikacji oraz ewentualnych pułapek.
Zamiast wymyślać koło na nowo, rozwiązaniem na wysyłanie e-maili jest użycie ``SwiftmailerBundle``,
który wykorzystuje moc biblioteki `Swiftmailer`_.

.. note::

    Przed użyciem nie zapomnij o włączeniu bundla w swoim kernelu::

        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            );

            // ...
        }

.. _swift-mailer-configuration:

Konfiguracja
------------

Przed użyciem Swiftmailer, należy pamiętać o jego konfiguracji.
Jedynym, obowiązkowym parametrem konfiguracyjnym jest ``transport``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport:  smtp
            encryption: ssl
            auth_mode:  login
            host:       smtp.gmail.com
            username:   nazwa_uzytkownika
            password:   twoje_haslo

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth-mode="login"
            host="smtp.gmail.com"
            username="nazwa_uzytkownika"
            password="twoje_haslo" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "nazwa_uzytkownika",
            'password'   => "twoje_haslo",
        ));

Dzięki konfiguracji Swiftmailer możemy zdecydować jak nasze maile będą wysłane do odbiorcy.

Dostępne są następujące atrybuty konfiguracyjne:

* ``transport``         (``smtp``, ``mail``, ``sendmail``, or ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption``        (``tls``, or ``ssl``)
* ``auth_mode``         (``plain``, ``login``, or ``cram-md5``)
* ``spool``

  * ``type`` (jaki typ kolejki wiadomości, aktualnie dostępna jest tylko opcja ``file`` )
  * ``path`` (gdzie przechowywać wiadomości)
* ``delivery_address``  (adres e-mail gdzie będą wysyłane WSZYSTKIE wiadomości)
* ``disable_delivery``  (ustaw na true jeśli chcesz wyłączyć wysyłanie e-maili)

Wysyłanie E-maili
---------------
Biblioteka Swiftmailer działa w ten sposób że są tworzone, konfigurowane a następnie wysyłane obiekty ``Swift_Message``.
"Mailer" jest odpowiedzialny za dostarczanie maili, dostępny jest on w usłudze ``mailer``.
Wysyłanie e-maili jest bardzo proste::

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

Aby utrzymać rzeczy bardziej "atomowe", treść e-maila jest trzymana w jego szablonie i renderowana przy użyciu metody ``renderView()``.

Obiekt ``$message`` obsługuje wiele opcji, takich jak dołączanie załączników,
dodawanie treści HTML, i wiele więcej.
Warto zapoznać się z dokumentacją Swiftmailer która dostarcza wiele informacji na temat tworzenia e-maili.

.. tip::

    Jest dostępnych kilka innych artykułów "cookbook" odnośnie wysyłania e-maili w Symfony2:

    * :doc:`gmail`
    * :doc:`email/dev_environment`
    * :doc:`email/spool`

.. _`Swiftmailer`: http://www.swiftmailer.org/
.. _`Tworzenie wiadomości`: http://swiftmailer.org/docs/messages
