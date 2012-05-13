.. index::
   single: Walidacja

Walidacja
=========

Walidacja to bardzo częste zadanie w aplikacjach webowych. Dane wprowadzone
do formularzy muszą być walidowane. Powinny one być walidowane jeszcze przed
zapisaniem ich do bazy danych czy przekazaniem ich do usługi web (ang. web service).

Symfony2 dostarcza komponent `Validator`_, dzięki któremu to zadanie jest łatwe i zrozumiałe.
Ten komponen oparty jest o `specyfikację JSR303 Bean Validation`_. Co? Specyfikacja Java
w PHP? Dobrze słyszysz, lecz nie jest to takie złe, jak to brzmi. Spójrzmy jak to może być
użyte w PHP.

.. index:
   single: Walidacja; Podstawy

Podstawy walidacji
------------------

Najlepszą drogą do zrozumienia walidacji jest zobaczyć to w akcji. Aby zacząć,
załóżmy, że utworzyłeś zwykły obiekt PHP, który musisz użyć gdzieś w swojej
aplikacji:

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }

Jak dotąd, jest to tylko zwyczajna klasa, która robi coś w Twojej aplikacji.
Celem walidacji jest powiedzieć Ci, czy zawartość obiektu jest prawidłowa.
Aby to działało, będziesz musiał skonfigurować listę zasad
(zwanych :ref:`constraints<validation-constraints>`), które obiekt musi spełniać,
aby przejść walidację. Te zasady mogą być określone przez wiele różnych
formatów (YAML, XML, adnotacje, czy PHP).

Na przykład, aby zagwarantować, że właściwość ``$name`` nie jest pusta,
dodaj poniższe:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: php-annotations

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

    Właściwości protected i private również mogą być walidowane, zarówno jak "gettery"
    (zobacz `validator-constraint-targets`).

.. index::
   single: Walidacja; Używanie walidatorów

Używanie usługi ``Validator``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dalej, aby w końcu walidować obiekt ``Author`` użyj metody ``validate`` z usługi
``validator`` (klasa :class:`Symfony\\Component\\Validator\\Validator`).
Zadanie ``validator`` jest proste: czytać reguły klasy i weryfikować, czy dane
znajdujące się w obiekcie spełniają te reguły. Jeśli walidacja się nie powiedzie,
zwracana jest tablica błędów. Spójrz na poniższy przykład z kontrolera:

.. code-block:: php

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

Jeśli właściwość ``$name`` jest pusta, ujrzysz poniższy komunikat błędu

.. code-block:: text

    Acme\BlogBundle\Author.name:
        Ta wartość nie może być pusta

Jeśli wstawisz jakąś wartość do właściwości ``$name``, pojawi się komunikat o
sukcesie.

.. tip::

    Przez większość czasu nie będziesz operował bezpośrednio z usługą
    ``validator`` czy musiał martwić się o wyświetlaniu błędów. Większość
    czasu będziesz używał walidacji pośrednio podczas obsługiwania danych
    wysłanego formularza. Po więcej informacji zobacz :ref:`book-validation-forms`.

Możesz również podać kolekcję błędów do szablonu.

.. code-block:: php

    if (count($errors) > 0) {
        return $this->render('AcmeBlogBundle:Author:validate.html.twig', array(
            'errors' => $errors,
        ));
    } else {
        // ...
    }

Wewnątrz szablonu, możesz wyświetlić listę błędów dokładnie tak, jak potrzebujesz:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Author/validate.html.twig #}

        <h3>The author has the following errors</h3>
        <ul>
        {% for error in errors %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Author/validate.html.php -->

        <h3>The author has the following errors</h3>
        <ul>
        <?php foreach ($errors as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach; ?>
        </ul>

.. note::

    Każdy błąd walidacji (zwany "naruszeniem zasad") reprezentowany jest przez obiekt
    :class:`Symfony\\Component\\Validator\\ConstraintViolation`.

.. index::
   single: Walidacja; Walidacja formularzy

.. _book-validation-forms:

Walidacja i formularze
~~~~~~~~~~~~~~~~~~~~~~

Usługa ``validator`` może być użyta kiedykolwiek do walidacji jakiegokolwiek obiektu.
Jednakże w rzeczywistości będziesz najczęściej używał walidatorów pośrednio,
podczas pracy z formularzami. Biblioteka formularzy Symfony używa usługi ``validator``
wewnętrznie, aby walidować obiekt formularza po tym, jak wartości zostały wysłane
i powiązane. Naruszenia zasad obiektu są konwertowane do obiektu ``FieldError``,
który może być łatwo wyświetlany wraz z formularzem. Typowy przebieg wysłania formularza
wygląda jak ten wycinek z kontrolera::

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

    W tym przykładzie użyto klasy formularza ``AuthorType``, która nie jest
    tutaj pokazana.

W celu uzyskania więcej informacji zobacz rozdział :doc:`Formularze</book/forms>`.

.. index::
   pair: Walidacja; Konfiguracja

.. _book-validation-configuration:

Konfiguracja
-------------

Walidator Symfony2 jest domyślnie włączony, jednakże musisz również włączyć
adnotacje, jeśli używasz metodę adnotacji do określania swoich reguł:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable_annotations="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array('validation' => array(
            'enable_annotations' => true,
        )));

.. index::
   single: Walidacja; Reguły

.. _validation-constraints:

Reguły
------

``Walidator`` jest zaprojektowany do walidacji obiektów pod względem reguł
(ang. ``constraints``).
Aby walidować obiekt, zmapuj jedną lub więcej reguł do jego klasy i przekaż go
do usługi ``validator``.

Za kulisami, reguła to prosty obiekt PHP będący stanowczym wyrażeniem.
W prawdziwym życiu, regułą może być: "Ciasto nie może być spalone". W Symfony2,
reguły są podobne: muszą być twierdzeniami, których warunkiem jest logiczna prawda
(ang. true). Pod względem wartości, reguła powie Ci, czy ta wartość spełnia
postawione przez Ciebie warunki, czy nie.

Wspierane reguły
~~~~~~~~~~~~~~~~

Symfony2 zawiera bardzo dużo najbardziej potrzebnych reguł:

.. include:: /reference/constraints/map.rst.inc

Możesz również stworzyć własne reguły. Ten temat jest szerzej omówiony
w artykule ":doc:`/cookbook/validation/custom_constraint`" cookbooka.

.. index::
   single: Walidacja; Konfiguracja reguł

.. _book-validation-constraint-configuration:

Konfiguracja reguł
~~~~~~~~~~~~~~~~~~

Niektóre reguły, takie jak :doc:`NotBlank</reference/constraints/NotBlank>,
są proste, podczas gdy inne, jak np. :doc:`Choice</reference/constraints/Choice>
mają kilka dostępnych opcji konfiguracji. Przyjmując, że klasa ``Autor`` ma pole
``płeć``, które może przyjmować wartość "kobieta" lub "mężczyzna":

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                plec:
                    - Choice: { choices: [mężczyzna, kobieta], message: Wybierz płeć. }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice(
             *     choices = { "mężczyzna", "kobieta" },
             *     message = "Wybierz płeć."
             * )
             */
            public $plec;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Autor">
                <property name="plec">
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

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Autor
        {
            public $plec;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('plec', new Choice(array(
                    'choices' => array('mężczyzna', 'kobieta'),
                    'message' => 'Wybierz płeć.',
                )));
            }
        }

.. _domyślne-opcje-walidacji:

Opcje zawsze mogą być przekazane do reguły w postaci tablicy. Jednakże niektóre
reguły pozwalają Ci podać *domyślną* wartość zamiast tablicy. W przypadku reguły
``Choice``, opcja ``choices`` może być w ten sposób określona.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                plec:
                    - Choice: [mężczyzna, kobieta]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Authr.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice({"mężczyzna", "kobieta"})
             */
            protected $plec;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="plec">
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
            protected $plec;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('plec', new Choice(array('mężczyzna', 'kobieta')));
            }
        }

Ma to jedynie na celu ułatwienie i przyspieszenie konfiguracji najczęściej używanych opcji reguł.

Jeśli nie jesteś pewien jak określić daną opcję, sprawdź dokumentację API dla reguł lub
zrób to bezpiecznie zawsze podając tablicę opcji (pierwsza metoda pokazana powyżej).

.. index::
   single: Walidacja; Targety reguł

.. _validator-constraint-targets:

Targety reguł
----------

Reguły mogą być zastosowane do właściwości klasy (np. ``imie``) lub do
publicznej metody getter (np. ``getImie``). Pierwszy sposób jest najpowszechniejszy
i najłatwiejszy w użyciu, lecz drugi wariant pozwala Ci na określenie bardziej
złożonych reguł walidacji.

.. index::
   single: Walidacja; Reguły właściwości

.. _validation-property-target:

Właściwości
~~~~~~~~~~~

Walidacja właściwości klasy jest najbardziej podstawową techniką walidacji.
Symfont2 pozwala Ci sprawdzać właściwości typu private, protected i public.
Poniższy listing pokazuje jak skonfigurować właściwość ``$imie`` klasy ``Autor``,
aby miała minimum 3 znaki.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                imie:
                    - NotBlank: ~
                    - MinLength: 3

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\NotBlank()
             * @Assert\MinLength(3)
             */
            private $imie;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <property name="imie">
                <constraint name="NotBlank" />
                <constraint name="MinLength">3</constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Autor
        {
            private $imie;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('imie', new NotBlank());
                $metadata->addPropertyConstraint('imie', new MinLength(3));
            }
        }

.. index::
   single: Walidacja; Reguły getterów

Gettery
~~~~~~~

Reguły mogą być również używane do zwracania wartości przez daną metodę.
Symfony2 pozwala Ci dodać regułę do jakiejkolwiek publicznej metody, której
nazwa zaczyna się od "get" lub "is". W tym przewodniku, oba z tych typów metod
są uznawane za gettery.

Zaletą tej techniki jest to, że pozwala Ci walidować Twój obiekt dynamicznie.
Na przykład, załóżmy że chcesz się upewnić, że wartość pola z hasłem nie jest
taka sama, jak imię użytkownika (z powodów bezpieczeństwa). Możesz zrobić to
tworząc metodę ``isPasswordLegal`` i wtedy zmapować, że metoda musi zwracać ``true``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            getters:
                passwordLegal:
                    - "True": { message: "Hasło nie może być takie samo jak Twoje imię" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\True(message = "Hasło nie może być takie samo jak Twoje imię")
             */
            public function isPasswordLegal()
            {
                // return true or false
            }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">Hasło nie może być takie samo jak Twoje imię</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Autor
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new True(array(
                    'message' => 'Hasło nie może być takie samo jak Twoje imię',
                )));
            }
        }

Teraz utwórz metodę ``isPasswordLegal`` i zastosuj w niej logikę, którą potrzebujesz::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    Bystrzejsi spośród Was na pewno zauważyli, że prefiks gettera ("get" lub "is")
    został pominięty w mapowaniu. To pozwala Ci na późniejsze używanie reguły
    dla właściwości o innej nazwie bez zmiany logiki walidacji.

.. _validation-class-target:

Klasy
~~~~~

Niektóre reguły dotyczą całej klasy podczas walidacji. Np. reguła
:doc:`Callback</reference/constraints/Callback>` jest ogólną regułą stosowaną
dla klasy. Podczas gdy klasa jest walidowana, metody określone
w regule są łatwo wykonywane, dzięki czemu każdy może dołożyć więcej swoich własnych
reguł.

.. _book-validation-validation-groups:

Grupy walidacji
---------------

Jak dotąd, nauczyłeś się jak dodać reguły do klasy i sprawdzić, czy klasa spełnia
wszystkie ze zdefiniowanych reguł. Jednakże w niektórych przypadkach będziesz
musiał walidować obiekt dla tylko *niektórych* reguł klasy. Aby to zrobić, możesz
zorganizować każdą regułę w jedną lub więcej "grup walidacji", a następnie zastosować
walidację dla wyłącznie jednej grupy reguł.

Na przykład, załóżmy że masz klasę ``User``, która jest używana zarówno kiedy
użytkownik się rejestruje, a także kiedy później edytuje swoje dane kontaktowe:

.. configuration-block::

    .. code-block:: yaml

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

Przy takiej konfiguracji, mamy tutaj dwie grupy walidacji:

* ``Default`` - zawiera reguły nieprzypisane do żadnej innej grupy;

* ``registration`` - zawiera reguły wyłącznie dla pól ``email`` i ``password``.

Aby powiedzieć walidatorowi, aby używał konkretnej grupy, podaj jedną lub więcej
nazw grup jako drugi argument metody ``validate()``::

    $errors = $validator->validate($author, array('registration'));

Oczywiście zazwyczaj będziesz pracował z niebezpośrednią walidacją formularzy.
Aby uzyskać więcej informacji jak używać grup walidacji dla formularzy, zobacz
:ref:`book-forms-validation-groups`.

.. index::
   single: Walidacja; Walidacja zwykłych wartości

.. _book-validation-raw-values:

Walidacja wartości i tablic
---------------------------

Jak dotąd, zobaczyłeś jak można walidować całe obiekty. Czasem jednak możesz
chcieć walidować zwykłą wartość - np. aby zweryfikować, czy dany ciąg znaków
jest poprawnym adresem email. Jest to również bardzo proste. Z poziomu kontrolera
wygląda to tak::

    // dpdaj to na początku klasy
    use Symfony\Component\Validator\Constraints\Email;
    
    public function addEmailAction($email)
    {
        $emailConstraint = new Email();
        // ustawiamy wszystkie opcje reuły
        $emailConstraint->message = 'Niepoprawny adres email';

        // używamy walidatora do sprawdzenia wartości
        $errorList = $this->get('validator')->validateValue($email, $emailConstraint);

        if (count($errorList) == 0) {
            // adres email jest poprawny
        } else {
            // to *nie* jest poprawny adres email
            $errorMessage = $errorList[0]->getMessage()
            
            // obsługa błędu
        }
        
        // ...
    }

Podczas wywoływania metody ``validateValue`` walidatora, możesz przekazać wartość
oraz obiekt reguły, który chcesz użyć do walidacji podanej wartości. Pełna lista
dostępnych reguł jest dostępna w sekcji :doc:`constraints reference</reference/constraints>`.

Metoda ``validateValue`` zwraca obiekt :class:`Symfony\\Component\\Validator\\ConstraintViolationList`,
który zachowuje się zupełnie jak tablica błędów. Każdy błąd w kolekcji jest obiektem
:class:`Symfony\\Component\\Validator\\ConstraintViolation` który przechowuje
komunikat błędu w swojej metodzie `getMessage`.

Myśli końcowe
-------------

``Walidator`` Symfony2 to potężne narzędzie, które może być wykorzystywane w celu
zagwarantowania, że dane jakiegokolwiek obiektu są "poprawne". Cała moc
walidacji leży w regułach, czyli zasadach, które możesz przypisać do właściwości
lub getterów Twojego obiektu. Pomimo, iż najczęściej będziesz używał walidacji
niebezpośrednio podczas używania formularzy, pamiętaj, że może ona być użyta
gdziekolwiek w celu walidacji jakiegokolwiek obiektu.

Dowiedz się więcej z Cookbooka
------------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _JSR303 Bean Validation specification: http://jcp.org/en/jsr/detail?id=303