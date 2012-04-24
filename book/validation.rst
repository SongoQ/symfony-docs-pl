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

This is purely meant to make the configuration of the most common option of
a constraint shorter and quicker.

If you're ever unsure of how to specify an option, either check the API documentation
for the constraint or play it safe by always passing in an array of options
(the first method shown above).

.. index::
   single: Validation; Constraint targets

.. _validator-constraint-targets:

Constraint Targets
------------------

Constraints can be applied to a class property (e.g. ``name``) or a public
getter method (e.g. ``getFullName``). The first is the most common and easy
to use, but the second allows you to specify more complex validation rules.

.. index::
   single: Validation; Property constraints

.. _validation-property-target:

Properties
~~~~~~~~~~

Validating class properties is the most basic validation technique. Symfony2
allows you to validate private, protected or public properties. The next
listing shows you how to configure the ``$firstName`` property of an ``Author``
class to have at least 3 characters.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - MinLength: 3

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\MinLength(3)
             */
            private $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="MinLength">3</constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint('firstName', new MinLength(3));
            }
        }

.. index::
   single: Validation; Getter constraints

Getters
~~~~~~~

Constraints can also be applied to the return value of a method. Symfony2
allows you to add a constraint to any public method whose name starts with
"get" or "is". In this guide, both of these types of methods are referred
to as "getters".

The benefit of this technique is that it allows you to validate your object
dynamically. For example, suppose you want to make sure that a password field
doesn't match the first name of the user (for security reasons). You can
do this by creating an ``isPasswordLegal`` method, and then asserting that
this method must return ``true``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - "True": { message: "The password cannot match your first name" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "The password cannot match your first name")
             */
            public function isPasswordLegal()
            {
                // return true or false
            }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">The password cannot match your first name</option>
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
                    'message' => 'The password cannot match your first name',
                )));
            }
        }

Now, create the ``isPasswordLegal()`` method, and include the logic you need::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    The keen-eyed among you will have noticed that the prefix of the getter
    ("get" or "is") is omitted in the mapping. This allows you to move the
    constraint to a property with the same name later (or vice versa) without
    changing your validation logic.

.. _validation-class-target:

Classes
~~~~~~~

Some constraints apply to the entire class being validated. For example,
the :doc:`Callback</reference/constraints/Callback>` constraint is a generic
constraint that's applied to the class itself. When that class is validated,
methods specified by that constraint are simply executed so that each can
provide more custom validation.

.. _book-validation-validation-groups:

Validation Groups
-----------------

So far, you've been able to add constraints to a class and ask whether or
not that class passes all of the defined constraints. In some cases, however,
you'll need to validate an object against only *some* of the constraints
on that class. To do this, you can organize each constraint into one or more
"validation groups", and then apply validation against just one group of
constraints.

For example, suppose you have a ``User`` class, which is used both when a
user registers and when a user updates his/her contact information later:

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

With this configuration, there are two validation groups:

* ``Default`` - contains the constraints not assigned to any other group;

* ``registration`` - contains the constraints on the ``email`` and ``password``
  fields only.

To tell the validator to use a specific group, pass one or more group names
as the second argument to the ``validate()`` method::

    $errors = $validator->validate($author, array('registration'));

Of course, you'll usually work with validation indirectly through the form
library. For information on how to use validation groups inside forms, see
:ref:`book-forms-validation-groups`.

.. index::
   single: Validation; Validating raw values

.. _book-validation-raw-values:

Validating Values and Arrays
----------------------------

So far, you've seen how you can validate entire objects. But sometimes, you
just want to validate a simple value - like to verify that a string is a valid
email address. This is actually pretty easy to do. From inside a controller,
it looks like this::

    // add this to the top of your class
    use Symfony\Component\Validator\Constraints\Email;
    
    public function addEmailAction($email)
    {
        $emailConstraint = new Email();
        // all constraint "options" can be set this way
        $emailConstraint->message = 'Invalid email address';

        // use the validator to validate the value
        $errorList = $this->get('validator')->validateValue($email, $emailConstraint);

        if (count($errorList) == 0) {
            // this IS a valid email address, do something
        } else {
            // this is *not* a valid email address
            $errorMessage = $errorList[0]->getMessage()
            
            // do something with the error
        }
        
        // ...
    }

By calling ``validateValue`` on the validator, you can pass in a raw value and
the constraint object that you want to validate that value against. A full
list of the available constraints - as well as the full class name for each
constraint - is available in the :doc:`constraints reference</reference/constraints>`
section .

The ``validateValue`` method returns a :class:`Symfony\\Component\\Validator\\ConstraintViolationList`
object, which acts just like an array of errors. Each error in the collection
is a :class:`Symfony\\Component\\Validator\\ConstraintViolation` object,
which holds the error message on its `getMessage` method.

Final Thoughts
--------------

The Symfony2 ``validator`` is a powerful tool that can be leveraged to
guarantee that the data of any object is "valid". The power behind validation
lies in "constraints", which are rules that you can apply to properties or
getter methods of your object. And while you'll most commonly use the validation
framework indirectly when using forms, remember that it can be used anywhere
to validate any object.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _JSR303 Bean Validation specification: http://jcp.org/en/jsr/detail?id=303