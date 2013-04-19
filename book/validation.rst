.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Walidacja

Walidacja
=========

Dane wprowadzone do formularzy wymagają weryfikacji. Dane wymagają również sprawdzenia
przed ich zapisem do bazy danych lub przekazaniem do serwisu internetowego.
Lecz nie chodzi tu tylko o prostą weryfikację danych. W tworzeniu aplikacji internetowych,
niezbędne jest wykazanie, że dane wprowadzane do aplikacji prowadzą do właściwych
wyników - to jest właśnie **walidacja**. `Walidacja`_ jest niezbędnym elementem
prawie wszystkich współczesnych procesów wytwórczych.  

Symfony2 dostarcza komponent `Validator`_, dzięki któremu to zadanie jest łatwe
 i zrozumiałe. Komponent ten oparty jest o `specyfikację JSR303 Bean Validation`_.
 Co? Specyfikacja Java w PHP? Nie przesłyszałeś się, lecz nie jest to takie złe,
 jakby sie mogło wydawać. Przyjrzyjmy się w jaki sposób może to być użyte w PHP.

.. index:
   single: walidacja; podstawy

Podstawy walidacji
------------------

Najlepszą sposobem do zrozumienia walidacji jest zobaczenie jej w działaniu.
Aby zacząć, załóżmy, że utworzyliśmy zwykły obiekt PHP, który musi się użyć gdzieś
w swojej aplikacji:

.. code-block:: php
   :linenos:
   
    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }

Jak dotąd, jest to tylko zwyczajna klasa, która robi coś w aplikacji.
Celem walidacji jest sprawdzenie, czy zawartość obiektu jest prawidłowa.
Aby to działało, trzeba skonfigurować listę reguł (zwanych
 :ref:`**ograniczeniami**, *ang. constraints*<validation-constraints>`),
 które obiekt musi spełniać, aby przejść walidację. Reguły te mogą być określone
 w wielu różnych formatach (YAML, XML, adnotacje, czy PHP).

Na przykład, aby zagwarantować, że właściwość ``$name`` nie jest pusta,
trzeba dodać następujący kod:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            public $name;
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
            }
        }

.. tip::

    Właściwości chronione i prywatne mogą być również walidowane, podobnie jak
    metody akcesorów (getery) – zobacz :ref:`validator-constraint-targets`.

.. index::
   single: walidacja; używanie walidatorów

Używanie usługi ``Validator``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Następnie, aby faktycznie walidować obiekt ``Author`` trzeba użyć metodę ``validate``
z usługi ``validator`` (klasy :class:`Symfony\\Component\\Validator\\Validator`).
Zadanie ``validator`` jest proste: odczytać ograniczenia i sprawdzić, czy dane
znajdujące się w obiekcie spełniają te ograniczenia. Jeśli walidacja się nie powiedzie,
zwracana jest tablica błędów. Rozpatrzmy poniższy przykład z wnetrza kontrolera:

.. code-block:: php
   :linenos:

    use Symfony\Component\HttpFoundation\Response;
    use Acme\BlogBundle\Entity\Author;
    // ...

    public function indexAction()
    {
        $author = new Author();
        // ... do something to the $author object

        $validator = $this->get('validator');
        $errors = $validator->validate($author);

        if (count($errors) > 0) {
            return new Response(print_r($errors, true));
        } else {
            return new Response('The author is valid! Yes!');
        }
    }

Jeśli właściwość ``$name`` jest pusta, to można zobaczyć poniższy komunikat błędu:

.. code-block:: text

    Acme\BlogBundle\Author.name:
        This value should not be blank

Jeśli wstawi się jakąś wartość do właściwości ``$name``, pojawi się komunikat o
sukcesie.

.. tip::

    Przeważnie nie trzeba korzystać bezpośrednio z usługi ``validator`` czy martwić
    się o wyświetlanie błędów. Zwykle  używa się walidację pośrednio podczas obsługiwania
    danych zgłaszanych w formularzu. Więcej informacji można znaleść w rozdziale
    :ref:`book-validation-forms`.

Możesz również przekazać kolekcję błędów do szablonu.

.. code-block:: php
   :linenos:

    if (count($errors) > 0) {
        return $this->render('AcmeBlogBundle:Author:validate.html.twig', array(
            'errors' => $errors,
        ));
    } else {
        // ...
    }

Jeśli zachodzi taka potrzeba, to można w szablonie wyprowadzić listę błędów:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/BlogBundle/Resources/views/Author/validate.html.twig #}

        <h3>The author has the following errors</h3>
        <ul>
        {% for error in errors %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/views/Author/validate.html.php -->

        <h3>The author has the following errors</h3>
        <ul>
        <?php foreach ($errors as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach; ?>
        </ul>

.. note::

    Każdy błąd walidacji (zwany "naruszeniem ograniczeń") reprezentowany jest przez
    obiekt :class:`Symfony\\Component\\Validator\\ConstraintViolation`.

.. index::
   single: walidacja; walidacja formularzy

.. _book-validation-forms:

Walidacja a formularze
~~~~~~~~~~~~~~~~~~~~~~

Usługa ``validator`` może być użyta do walidacji dowolnego obiektu w dowolnym momencie.
Jednakże w rzeczywistości najczęściej używa się walidatorów pośrednio,
podczas pracy z formularzami. Biblioteka formularzy Symfony używa usługi ``validator``
wewnętrznie do sprawdzenia odnośnego obiektu formularza po tym, jak wartości zostaną
zgłoszone i powiązane. Naruszenia ograniczeń dla obiektu są konwertowane do obiektu
``FieldError``, który może być łatwo wyświetlany wraz z formularzem. Typowa procedura
wysłania formularza z poziomu kontrolera wyglada następująco::

    use Acme\BlogBundle\Entity\Author;
    use Acme\BlogBundle\Form\AuthorType;
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function updateAction(Request $request)
    {
        $author = new Acme\BlogBundle\Entity\Author();
        $form = $this->createForm(new AuthorType(), $author);

        if ($request->getMethod() == 'POST') {
            $form->bindRequest($request);

            if ($form->isValid()) {
                // the validation passed, do something with the $author object

                return $this->redirect($this->generateUrl('...'));
            }
        }

        return $this->render('BlogBundle:Author:form.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. note::

    W tym przykładzie użyto klasę formularza ``AuthorType``, która nie jest
    tutaj pokazana.

W celu uzyskania więcej informacji zobacz do rozdziału :doc:`Formularze</book/forms>`.

.. index::
   pair: walidacja; konfiguracja

.. _book-validation-configuration:

Konfiguracja
------------

Walidator Symfony2 jest domyślnie włączony, jednak gdy chce się stosować metodę
adnotacji, to należy określić to w ograniczeniach:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable_annotations="true" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array('validation' => array(
            'enable_annotations' => true,
        )));

.. index::
   single: walidacja; ograniczenia
   pair: walidacja; asercje

.. _validation-constraints:

Ograniczenia
------------

Usługa ``validator`` jest zaprojektowana do weryfikacji obiektów w oparciu
o **ograniczenia** (*ang. ``constraints``*). W celu walidacji obiektu trzeba tylko
odwzorować jedno lub więcej ograniczeń na tą klasę  i następnie przekazać to do
usługi ``validator``.

Ograniczenie to prosty obiekt PHP, który wykonuje wyrażenie asercyjne (*ang.
assertive statement*). `Asercja`_ w programowaniu, to wyrażenie lub metoda pozwalająca
sprawdzić  prawdziwość twierdzeń dokonanych odnośnie jakichś aspektów systemu lub
jego elementów. W prawdziwym życiu, takim twierdzeniem może być zdanie: "Ciasto
nie może być spalone". W Symfony2, ograniczenia są podobne: są to twierdzenia
(asercje), że warunek jest spełniony.

Obsługiwane ograniczenia
~~~~~~~~~~~~~~~~~~~~~~~~

Pakiety Symfony2 udostępniają większość z powszechnie używanych ograniczeń:

.. include:: /reference/constraints/map.rst.inc

Można również stworzyć własne reguły. Ten temat jest szerzej omówiony
w artykule ":doc:`/cookbook/validation/custom_constraint`".

.. index::
   single: walidacja; konfiguracja

.. _book-validation-constraint-configuration:

Konfiguracja ograniczeń
~~~~~~~~~~~~~~~~~~~~~~~

Niektóre ograniczenia, takie jak :doc:`NotBlank</reference/constraints/NotBlank>,
są proste, podczas gdy inne, jak np. :doc:`Choice</reference/constraints/Choice>
mają kilka dostępnych opcji konfiguracji. Załóżmy, że klasa ``Author`` ma właściwość
``gender``, które może przyjmować wartość "kobieta" lub "mężczyzna":

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { choices: [mężczyzna, kobieta], message: Wybierz płeć. }

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(
             *     choices = { "mężczyzna", "kobieta" },
             *     message = "Wybierz płeć."
             * )
             */
            public $gender;
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>mężczyzna</value>
                            <value>kobieta</value>
                        </option>
                        <option name="message">Wybierz płeć.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array(
                    'choices' => array('mężczyzna', 'kobieta'),
                    'message' => 'Wybierz płeć.',
                )));
            }
        }

.. _validation-default-option:

Opcje ograniczeń mogą być również przekazywane w tablicy. Niektóre ograniczenia
pozwalają także przekazać wartość "domyślną" opcji zamiast tablicy.
W przypadku ograniczenia ``Choice`` opcje ``choices`` może zostać określona
w ten sposób.

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: [mężczyzna, kobieta]

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice({"mężczyzna", "kobieta"})
             */
            protected $gender;
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <value>mężczyzna</value>
                        <value>kobieta</value>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array('mężczyzna', 'kobieta')));
            }
        }

Ma to jedynie na celu ułatwienie i przyspieszenie konfiguracji najczęściej
używanych opcji ograniczeń.

Jeśli czasem nie będziesz pewien jak określić opcję, sprawdź dokumentację API
dla ograniczeń lub postępuj bezpiecznie przekazując w tablicy opcji (pierwsza
powyżej pokazana metoda).


Tłumaczenie komunikatów ograniczeń
----------------------------------

Pełną informację o tłumaczeniu komunikatów ograniczeń znajdziesz w dokumencie
:ref:`book-translation-constraint-messages`.


.. index::
   single: walidacja; cele ograniczeń


.. _validator-constraint-targets:

Cele ograniczeń
---------------

Ograniczenia można stosować do właściwości klasy (np. ``name``) lub publicznych
metod aksesorów (np.``getFullName``). Pierwsze zastosowanie jest najbardziej
rozpowszechnione i łatwe w użyciu, lecz drugie pozwala określić bardziej złożone
reguły walidacji.

.. index::
   single: walidacja; właściwości ograniczeń

.. _validation-property-target:

Właściwości
~~~~~~~~~~~

Walidacja właściwości klas jest dość rozpowszechnioną a techniką walidacji.
Symfony2 pozwala sprawdzać prywatne, chronione i publiczne właściwości.
Następujący listing pokazuje jak skonfigurować właściwość ``$firstName``
klasy ``Author``, której wartość powinna mieć co najmniej 3 znaki.

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - Length:
                        min: 3

    .. code-block:: php-annotations
       :linenos:

        // Acme/BlogBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\Length(min = "3")
             */
            private $firstName;
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="Length">
                    <option name="min">3</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Length;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint(
                    'firstName',
                    new Length(array("min" => 3)));
            }
        }

.. index::
   single: walidacja; ograniczenia dla akcesorów
   pair: walidacja; akcesory

Metody akcesory
~~~~~~~~~~~~~~~

Ograniczenia mogą również być stosowane do zwrócenia wartości metody.
Symfony2 umożliwia dodanie ograniczenia do jakiejkolwiek publicznej metody,
której nazwa zaczyna się od "get" lub "is". W typ podręczniku oba takie typy
metod określane są jako akcesory (getery i isery) (*ang. getters, issers*).

Zaletą tej techniki jest to, że pozwala dynamicznie walidować obiekt. Przykładowo
załóżmy, że chcemy się upewnić, że pole hasła nie zgadza się z imieniem użytkownika
(w celach bezpieczeństwa). Można to uczynić przez stworzenie metody ``isPasswordLegal``
i następnie zrobić załóżenie, że metoda ta musi zwrócić ``true``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - "True": { message: "Hasło nie może być takie samo jak Twoje imię" }

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\True(message = "Hasło nie może być takie samo jak Twoje imię")
             */
            public function isPasswordLegal()
            {
                // zwraca true lub false
            }
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">Hasło nie może być takie samo jak Twoje imię</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new True(array(
                    'message' => 'Hasło nie może być takie samo jak Twoje imię',
                )));
            }
        }

Teraz utwórzmy metodę ``isPasswordLegal()`` i dołączmy potrzebną logikę::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    Niektórzy z czytelników mogli zauważyć, że przedrostek akcesora ("get" lub "is")
    jest pomijany w odwzorowaniu. Pozwala to później przenieść ograniczenie do
    właściwości o tej samej nazwie (lub odwrotnie) bez zmiany logiki walidacji.

.. _validation-class-target:

Klasy
~~~~~

Niektóre ograniczenia mogą być zastosowanie do walidacji całej klasy. Na przykład,
ograniczenie :doc:`*Callback*</reference/constraints/Callback>` jest ogólnym
ograniczeniem stosowanym do całej klasy. Podczas walidacji klasy, wykonywane
są metody określone przez ograniczenie, więc każda może dostarczyć niestandardowej
walidacji.

.. _book-validation-validation-groups:

Grupy walidacji
---------------

Dotychczas byliśmy w stanie dodać ograniczenia do klasy i zapytać, czy klasa spełnia
wszystkie określone ograniczenia. Jednak w niektórych przypadkach zachodzi potrzeba
potwierdzenia tylko niektórych z tych ograniczeń w klasie. Aby to wykonać, trzeba
zorganizować każde z ograniczeń w jednej lub więcej "grup walidacyjnych" i następnie
zastosować walidację tylko na jednej z takich grup.

Przykładowo załóżmy, że mamy klasę User, która jest stosowana zarówno podczas
rejestracji użytkownika jak i podczas aktualizowania jego informacji kontaktowych:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\User:
            properties:
                email:
                    - Email: { groups: [registration] }
                password:
                    - NotBlank: { groups: [registration] }
                    - MinLength: { limit: 7, groups: [registration] }
                city:
                    - MinLength: 2

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface;
        use Symfony\Component\Validator\Constraints as Assert;

        class User implements UserInterface
        {
            /**
            * @Assert\Email(groups={"registration"})
            */
            private $email;

            /**
            * @Assert\NotBlank(groups={"registration"})
            * @Assert\MinLength(limit=7, groups={"registration"})
            */
            private $password;

            /**
            * @Assert\MinLength(2)
            */
            private $city;
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\User">
            <property name="email">
                <constraint name="Email">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="password">
                <constraint name="NotBlank">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
                <constraint name="MinLength">
                    <option name="limit">7</option>
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="city">
                <constraint name="MinLength">7</constraint>
            </property>
        </class>

    .. code-block:: php
       :linenos:

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('email', new Email(array(
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('password', new NotBlank(array(
                    'groups' => array('registration')
                )));
                $metadata->addPropertyConstraint('password', new MinLength(array(
                    'limit'  => 7,
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('city', new MinLength(3));
            }
        }

W tej konfiguracji są dwie grupy walidacyjne:

* ``Default`` - zawiera ograniczenia nieprzypisane do żadnej innej grupy;

* ``registration`` - zawiera ograniczenia wyłącznie dla pól ``email`` i ``password``.

Aby poinstruować walidator o stosowaniu określonej grupy, trzeba przekazać jedną
lub więcej nazw grup jako drugi argument metody ``validate()``::

    $errors = $validator->validate($author, array('registration'));

Oczywiście będziesz zazwyczaj używał walidacji pośrednio, poprzez bibliotekę formularzy.
Więcej informacji o tym jak używać grup walidacyjnych w formularzach znajdziesz w rozdziale
:ref:`book-forms-validation-groups`.

.. index::
   single: walidacja; walidacja wartości
   single: walidacja; walidacja tablic

.. _book-validation-raw-values:

Walidacja wartości i tablic
---------------------------

Dotąd widzieliśmy, jak można sprawdzać całe obiekty. Lecz czasami zachodzi potrzeba
sprawdzenia tylko pojedynczej wartości – jak przykładowo w łańcuchu tekstowym, który
powinien być prawidłowym adresem e-mail. W rzeczywistości jest to bardzo łatwe do
zrobienia. Wewnątrz kontrolera wygląda to podobnie do tego::

    use Symfony\Component\Validator\Constraints\Email;
    // ...

    public function addEmailAction($email)
    {
        $emailConstraint = new Email();
        // wszystkie "opcje" ograniczeń mogą zostać ustawione w ten sposób
        $emailConstraint->message = 'Invalid email address';

        // użycie walidatora do walidacji walidacji wartości
        $errorList = $this->get('validator')->validateValue(
            $email,
            $emailConstraint
        );

        if (count($errorList) == 0) {
            // jest to prawidłowy adres email, zrobienie czegoś
        } else {
            // to nie jest prawidłowy adres email
            $errorMessage = $errorList[0]->getMessage();

            // ... zrobienie czegoś z błędem
        }

        // ...
    }

Wywołując w walidatorze metodę ``validateValue`` można przekazać tam surową wartość
i obiekt ograniczenia, jakie chce się walidować. Pełną listę dostępnych ograniczeń,
z pełną nazwą klasy dla każdego ograniczenia, znajdziesz w rozdziale
 :doc:`Informacje o ograniczeniach</reference/constraints>`.

Metoda ``validateValue`` zwraca obiekt
:class:`Symfony\\Component\\Validator\\ConstraintViolationList`,
który zachowuje się zupełnie jak tablica błędów. Każdy błąd w kolekcji jest obiektem
:class:`Symfony\\Component\\Validator\\ConstraintViolation` który przechowuje
komunikat błędu w swojej metodzie `getMessage`.

Wnioski końcowe
---------------

Usługa ``validator``Symfony2 jest zaawansowanym narzędziem, które można wykorzystać
do zagwarantowania poprawności danych każdego obiektu. Siła walidacji bierze się
z "ograniczeń", które są regułami jakie muszą spełniać właściwości lub metody
akcesory w obiekcie. Najczęściej będziesz stosował framework walidacyjny pośrednio,
podczas używania formularzy, pamiętając, że walidacja może być stosowana gdziekolwiek,
w celu sprawdzenia poprawności danych dowolnego obiektu.

Dowiedz się więcej w Receptariuszu
----------------------------------

* :doc:`Jak utworzyć własne ograniczenia walidacyjne</cookbook/validation/custom_constraint>`

.. _`Validator`: https://github.com/symfony/Validator
.. _`JSR303 Bean Validation specification`: http://jcp.org/en/jsr/detail?id=303
.. _`Walidacja`: http://pl.wikipedia.org/wiki/Walidacja_(technika)
.. _`Asercja`: http://pl.wikipedia.org/wiki/Asercja_(informatyka)