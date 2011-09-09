.. index::
   single: Security; Configuration Reference

Konfiguracja Bezpieczeństwa (Security)
======================================

System bezpieczeństwa jest jedną z najpotężniejszych części Symfony2, i może
w dużej mierze być kontrolowane poprzez swoją konfigurację.

Pełna Domyślna Konfiguracja
---------------------------

Poniżej znajduje się pełna domyślna konfiguracja dla systemu bezpieczeństwa.
Każda z sekcji zostanie wyjaśniona w następnej sekcji.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_denied_url: /foo/error403

            always_authenticate_before_granting: false

            # możliwe strategie: none, migrate, invalidate
            session_fixation_strategy: migrate

            access_decision_manager:
                strategy: affirmative
                allow_if_all_abstain: false
                allow_if_equal_granted_denied: true

            acl:
                connection: default # każda nazwa skonfigurowana w sekcji doctrine.dbal 
                tables:
                    class: acl_classes
                    entry: acl_entries
                    object_identity: acl_object_identities
                    object_identity_ancestors: acl_object_identity_ancestors
                    security_identity: acl_security_identities
                cache:
                    id: service_id
                    prefix: sf2_acl_
                voter:
                    allow_if_object_identity_unavailable: true

            encoders:
                somename:
                    class: Acme\DemoBundle\Entity\User
                Acme\DemoBundle\Entity\User: sha512
                Acme\DemoBundle\Entity\User: plaintext
                Acme\DemoBundle\Entity\User:
                    algorithm: sha512
                    encode_as_base64: true
                    iterations: 5000
                Acme\DemoBundle\Entity\User:
                    id: my.custom.encoder.service.id

            providers:
                memory:
                    name: memory
                    users:
                        foo: { password: foo, roles: ROLE_USER }
                        bar: { password: bar, roles: [ROLE_USER, ROLE_ADMIN] }
                entity:
                    entity: { class: SecurityBundle:User, property: username }

            factories:
                MyFactory: %kernel.root_dir%/../src/Acme/DemoBundle/Resources/config/security_factories.xml

            firewalls:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: name
                    context: name
                    stateless: false
                    x509:
                        provider: name
                    http_basic:
                        provider: name
                    http_digest:
                        provider: name
                    form_login:
                        check_path: /login_check
                        login_path: /login
                        use_forward: false
                        always_use_default_target_path: false
                        default_target_path: /
                        target_path_parameter: _target_path
                        use_referer: false
                        failure_path: /foo
                        failure_forward: false
                        failure_handler: some.service.id
                        success_handler: some.service.id
                        username_parameter: _username
                        password_parameter: _password
                        csrf_parameter: _csrf_token
                        csrf_page_id: form_login
                        csrf_provider: my.csrf_provider.id
                        post_only: true
                        remember_me: false
                    remember_me:
                        token_provider: name
                        key: someS3cretKey
                        name: NameOfTheCookie
                        lifetime: 3600 # in seconds
                        path: /foo
                        domain: somedomain.foo
                        secure: true
                        httponly: true
                        always_remember_me: false
                        remember_me_parameter: _remember_me
                    logout:
                        path:   /logout
                        target: /
                        invalidate_session: false
                        delete_cookies:
                            a: { path: null, domain: null }
                            b: { path: null, domain: null }
                        handlers: [some.service.id, another.service.id]
                        success_handler: some.service.id
                    anonymous: ~

            access_control:
                -
                    path: ^/foo
                    host: mydomain.foo
                    ip: 192.0.0.0/8
                    roles: [ROLE_A, ROLE_B]
                    requires_channel: https

            role_hierarchy:
                ROLE_SUPERADMIN: ROLE_ADMIN
                ROLE_SUPERADMIN: 'ROLE_ADMIN, ROLE_USER'
                ROLE_SUPERADMIN: [ROLE_ADMIN, ROLE_USER]
                anything: { id: ROLE_SUPERADMIN, value: 'ROLE_USER, ROLE_ADMIN' }
                anything: { id: ROLE_SUPERADMIN, value: [ROLE_USER, ROLE_ADMIN] }

.. _reference-security-firewall-form-login:

Konfiguracja Formularza Logowania
---------------------------------

Gdy używasz słuchacza uwierzytelniania ``form_login`` pod firewallem,
dostępnych jest kilka niestandardowych opcji dla skonfigurowania zdarzeń
w "formularzu logowania":

Formularz oraz Proces Logowania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*   ``login_path`` (typ: ``string``, domyślnie: ``/login``)
    Jest to URL do którego zostanie przekierowany użytkownik (chyba że ``use_forward``
    jest ustawione na ``true``) kiedy on/ona stara się uzyskać dostęp do chronionego
    zasobu ale nie jest w pełni uwierzytelniony.

    Ten URL **musi** być dostępny przez normalnego użytkownika, nieuwierzytelnionego użytkownika,
    inaczej możesz stworzyć pętlę przekierowań. W celu uzyskania większej ilości informacji,
    zobacz ":ref:`Avoid Common Pitfalls<book-security-common-pitfalls>`".

*   ``check_path`` (typ: ``string``, domyślnie: ``/login_check``)
    Jest to URL do którego musi być wysyłany Twój formularz logowania. 
    Firewall będzie przechwytywał wszystkie zapytania (domyślnie tylko ``POST``)
    do tego URLa w celu sprawdzenia uprawnień wysłanych danych.

    Upewnij się czy ten URL znajduje się w głównym firewallu (np. nie twórz osobnego
    firewalla tylko dla URLa ``check_path``).

*   ``use_forward`` (typ: ``Boolean``, domyślnie: ``false``)
    Jeśli wolisz aby użytkownik został przeniesiony (forward) do formularza logowania
    zamiast przekierowanym (redirect), ustaw tą opcję na ``true``.

*   ``username_parameter`` (typ: ``string``, domyślnie: ``_username``)
    Jest to nazwa pola które powinieneś użyć dla pola reprezentującego
    nazwę użytkownika w swoim formularzu logowania. Gdy zostanie wysłany formularz
    logowania na ``check_path``, system bezpieczeństwa będzie szukał parametru
    o tej nazwie w tablicy POST.

*   ``password_parameter`` (typ: ``string``, domyślnie: ``_password``)
    Jest to nazwa pola które powinieneś użyć dla pola reprezentującego
    hasło użytkownika w swoim formularzu logowania. Gdy zostanie wysłany formularz
    logowania na ``check_path``, system bezpieczeństwa będzie szukał parametru
    o tej nazwie w tablicy POST.

*   ``post_only`` (typ: ``Boolean``, domyślnie: ``true``)
    Domyślnie, musisz wysłać swój formularz logowania na URL ``check_path`` jako
    zapytanie typu POST. Jeśli ustawisz tą opcję na ``true``, będziesz mógł wysyłać
    także zapytania typu GET na URL ``check_path``.

Przekierowanie po Zalogowaniu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* ``always_use_default_target_path`` (typ: ``Boolean``, domyślnie: ``false``)
* ``default_target_path`` (typ: ``string``, domyślnie: ``/``)
* ``target_path_parameter`` (typ: ``string``, domyślnie: ``_target_path``)
* ``use_referer`` (typ: ``Boolean``, domyślnie: ``false``)
