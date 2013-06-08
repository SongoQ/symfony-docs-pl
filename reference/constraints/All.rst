All
===


Po zastosowaniu do jakiejś tablicy (lub obiektu po którym można dokonywać trawersacji),
ograniczenie to umożliwia zastosować kolekcję ograniczeń do każdego elementy tej tablicy.


+-----------+-------------------------------------------------------------------+
| Dotyczy   | :ref:`właściwości lun metody<validation-property-target>`         |
+-----------+-------------------------------------------------------------------+
| Opcje     | - `constraints`_                                                  |
+-----------+-------------------------------------------------------------------+
| Klasa     | :class:`Symfony\\Component\\Validator\\Constraints\\All`          |
+-----------+-------------------------------------------------------------------+
| Walidator | :class:`Symfony\\Component\\Validator\\Constraints\\AllValidator` |
+-----------+-------------------------------------------------------------------+

Podstawowe zastosowanie
-----------------------

Załóżmy, że mamy tablicę łańcuchów tekstowych i chcemy sprawdzić każdego wpis w tej tablicy:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                favoriteColors:
                    - All:
                        - NotBlank:  ~
                        - Length:
                            min: 5

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;
  
        class User
        {
            /**
             * @Assert\All({
             *     @Assert\NotBlank
             *     @Assert\Length(min = "5"),
             * })
             */
             protected $favoriteColors = array();
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Entity\User">
            <property name="favoriteColors">
                <constraint name="All">
                    <option name="constraints">
                        <constraint name="NotBlank" />
                        <constraint name="Length">
                            <option name="min">5</option>
                        </constraint>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php
       :linenos:

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;
       
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('favoriteColors', new Assert\All(array(
                    'constraints' => array(
                        new Assert\NotBlank(),
                        new Assert\Length(array('min' => 5)),
                    ),
                )));
            }
        }

Teraz każdy wpis w tablicy ``favoriteColors`` będzie sprawdzony pod kątem tego,
czy nie jest pusty i czy ma co najmniej 5 znaków.

Opcje
-----

constraints
~~~~~~~~~~~

**typ**: ``array`` [:ref:`default option<validation-default-option>`]

Ta obowiązkowa opcja jest tablicą ograniczeń walidacyjnych, które chce się
zastosować dla każdego elementu danej tablicy.

