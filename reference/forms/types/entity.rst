.. index::
   single: Forms; Fields; choice

entity Typ Pola
===============

Specjalna wersja pola ``choice`` która jest zaprojektowana do ładowania opcji z encji
Doctrine. Dla przykładu, jeśli masz encję ``Category``, mógłbyś użyć tego pola do
wyświetlenia pola ``select`` z wszystkimi, lub kilkoma, obiektami ``Category``
z bazy danych.

+------------------+---------------------------------------------------------------------------------------+
| Renderowane jako | może być generowane na kilka sposobów (zobacz :ref:`forms-reference-choice-tags`)     |
+------------------+---------------------------------------------------------------------------------------+
| Opcje            | - `class`_                                                                            |
|                  | - `property`_                                                                         |
|                  | - `query_builder`_                                                                    |
|                  | - `em`_                                                                               |
+------------------+---------------------------------------------------------------------------------------+
| Odziedziczone    | - `required`_                                                                         |
| opcje            | - `label`_                                                                            |
|                  | - `multiple`_                                                                         |
|                  | - `expanded`_                                                                         |
|                  | - `preferred_choices`_                                                                |
|                  | - `empty_value`_                                                                      |
|                  | - `read_only`_                                                                        |
|                  | - `error_bubbling`_                                                                   |
+------------------+---------------------------------------------------------------------------------------+
| Rodzic           | :doc:`choice</reference/forms/types/choice>`                                          |
+------------------+---------------------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Bridge\\Doctrine\\Form\\Type\\EntityType`                            |
+------------------+---------------------------------------------------------------------------------------+

Podstawowe Użycie
-----------------

Typ ``entity`` posiada tylko jedną wymaganą opcję: encję która powinna być wylistowana
w polu wyboru::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
    ));

W tym przypadku, wszystkie obiekty ``User`` zostaną pobrane z bazy danych i wyrenderowane
w jednym polu ``select``, przyciskach radio lub liście checkboxów (to zależy od wartości opcji
``multiple`` oraz ``expanded``).

Używanie Własnego Zapytania dla Encji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli potrzebujesz zdefiniować własne zapytanie do wyciągania encji (np. chcesz zwrócić tylko
kilka encji, lub chcesz je posortować), użyj opcji ``query_builder``. Najprostszym sposobem
użycia tej opcji jest::

    use Doctrine\ORM\EntityRepository;
    // ...

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'query_builder' => function(EntityRepository $er) {
            return $er->createQueryBuilder('u')
                ->orderBy('u.username', 'ASC');
        },
    ));

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Opcje Pola
----------

class
~~~~~

**typ**: ``string`` **wymagana**

Nazwa klasy Twojej encji (np. ``AcmeStoreBundle:Category``). Może to być
w pełni kwalifikowana nazwa klasy (np. ``Acme\StoreBundle\Entity\Category``)
lub też jej krótszy alias (jak pokazano wcześniej).

property
~~~~~~~~

**typ**: ``string``

Jest to metoda która powinna być użyta do wyświetlania encji
jako tekst w elementach HTML. Jeśli pozostawisz puste, obiekt encji
będzie rzutowany na string, w tym przypadku obiekt musi posiadać metodę
``__toString()``.

query_builder
~~~~~~~~~~~~~

**typ**: ``Doctrine\ORM\QueryBuilder`` lub Closure

Jeśli zdefiniowano, zostanie użyta do wczytania zbioru opcji (oraz ich
kolejności) które mają być użyte w polu. Wartośc tej opcji może być obiektem
``QueryBuilder`` lub Closure. Jeśli użyto Closure, powinien zostać przekazany
jeden argument, który jest ``EntityRepository`` encji.

em
~~

**typ**: ``string`` **domyślnie**: domyślny menedżer encji

Jeśli zdefiniowano, zdefiniowany menedżer encji zostanie użyty do wczytania
opcji wyboru zamiast domyślnego menedżera.

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
