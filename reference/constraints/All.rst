All
===


"Ograniczenie" (reguła walidacji) to ma zastosowane do tablic (lub obiektów typu "Traversable"), pozwala
na przypisanie kolekcji "ograniczeń" dla każdego elementu tablicy.

+----------------+------------------------------------------------------------------------+
| Dotyczy        | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opcje          | - `constraints`_                                                       |
+----------------+------------------------------------------------------------------------+
| Klasa          | :class:`Symfony\\Component\\Validator\\Constraints\\All`               |
+----------------+------------------------------------------------------------------------+
| Walidator      | :class:`Symfony\\Component\\Validator\\Constraints\\AllValidator`      |
+----------------+------------------------------------------------------------------------+

Podstawowe użycie
-----------------

Przypuśćmy że chcesz zwalidować każdy element w tablicy napisów.
Możesz to uczynić poprzez następującą konfiguracje:

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                favoriteColors:
                    - All:
                        - NotBlank:  ~
                        - MinLength: 5

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Entity/User.php
       namespace Acme\UserBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class User
       {
           /**
            * @Assert\All({
            *     @Assert\NotBlank
            *     @Assert\MinLength(5),
            * })
            */
            protected $favoriteColors = array();
       }

Poprzez te ustawienia, każdy element w tablicy ``favoriteColors`` zostanie zwalidowany.
W tym konkretnym przypadku element tablicy nie może być pusty oraz musi zawierać co najmniej 5 znaków. 

Opcje
-----

constraints
~~~~~~~~~~~

**typ**: ``array`` [:ref:`default option<validation-default-option>`]

Jest to opcja wymagana która powinna być tablicą "ograniczeń" które będą zastosowane 
dla każdego elementu tablicy dla której zdefiniujesz to "ograniczenie". 

