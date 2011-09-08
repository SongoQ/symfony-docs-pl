Walidacja poprzez Ograniczenia
==============================

.. toctree::
   :maxdepth: 1
   :hidden:

   constraints/NotBlank
   constraints/Blank
   constraints/NotNull
   constraints/Null
   constraints/True
   constraints/False
   constraints/Type

   constraints/Email
   constraints/MinLength
   constraints/MaxLength
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Max
   constraints/Min

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/Callback
   constraints/Valid
   constraints/All

Walidator jest zaprojektowany aby walidować obiekty poprzez *ograniczenia*.
W prawdziwym życiu, ograniczeniem może być: "Ciasto nie może zostać spalone".
W Symfony2, ograniczenia są bardzo podobne: Są to twierdzenia których warunek
jest prawdziwy.

Obsługiwane Ograniczenia
------------------------

Następujące ograniczenia są natywnie dostępne w Symfony2:

Podstawowe Ograniczenia
-----------------------

Są to podstawowe ograniczenia: używaj ich do dochodzenia bardzo prostych 
stwierdzeń o wartości własności lub zwracanej wartości metod Twojego obiektu.

* :doc:`NotBlank <constraints/NotBlank>`
* :doc:`Blank <constraints/Blank>`
* :doc:`NotNull <constraints/NotNull>`
* :doc:`Null <constraints/Null>`
* :doc:`True <constraints/True>`
* :doc:`False <constraints/False>`
* :doc:`Type <constraints/Type>`


Ograniczenia Znakowe
~~~~~~~~~~~~~~~~~~~~

* :doc:`Email <constraints/Email>`
* :doc:`MinLength <constraints/MinLength>`
* :doc:`MaxLength <constraints/MaxLength>`
* :doc:`Url <constraints/Url>`
* :doc:`Regex <constraints/Regex>`
* :doc:`Ip <constraints/Ip>`

Ograniczenia Numeryczne
~~~~~~~~~~~~~~~~~~~~~~~

* :doc:`Max <constraints/Max>`
* :doc:`Min <constraints/Min>`

Ograniczenia Daty
~~~~~~~~~~~~~~~~~

* :doc:`Date <constraints/Date>`
* :doc:`DateTime <constraints/DateTime>`
* :doc:`Time <constraints/Time>`

Ograniczenia Kolekcji
~~~~~~~~~~~~~~~~~~~~~

* :doc:`Choice <constraints/Choice>`
* :doc:`Collection <constraints/Collection>`
* :doc:`UniqueEntity <constraints/UniqueEntity>`
* :doc:`Language <constraints/Language>`
* :doc:`Locale <constraints/Locale>`
* :doc:`Country <constraints/Country>`

Ograniczenia Pliku
~~~~~~~~~~~~~~~~~~

* :doc:`File <constraints/File>`
* :doc:`Image <constraints/Image>`

Inne Ograniczenia
~~~~~~~~~~~~~~~~~

* :doc:`Callback <constraints/Callback>`
* :doc:`All <constraints/All>`
* :doc:`Valid <constraints/Valid>`
