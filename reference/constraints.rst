Walidacja poprzez ograniczenia
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
   constraints/Length
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Range

   constraints/EqualTo
   constraints/NotEqualTo
   constraints/IdenticalTo
   constraints/NotIdenticalTo
   constraints/LessThan
   constraints/LessThanOrEqual
   constraints/GreaterThan
   constraints/GreaterThanOrEqual

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/Count
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/CardScheme
   constraints/Luhn
   constraints/Iban
   constraints/Isbn
   constraints/Issn

   constraints/Callback
   constraints/All
   constraints/UserPassword
   constraints/Valid

Walidator został zaprojektowany do sprawdzania prawidłowości obiektów w stosunku
do *ograniczeń*. W codziennym życiu ograniczeniem może być zasada: "To ciasto nie
może być spalone". W Symfony2 ograniczenia są podobne: są twierdzeniami,
że warunek jest prawdziwy.

Obsługiwane ograniczenia
------------------------

W Symfony2 natywnie dostępne są następujące ograniczenia:

.. include:: /reference/constraints/map.rst.inc
