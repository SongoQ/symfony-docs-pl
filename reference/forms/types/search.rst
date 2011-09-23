.. index::
   single: Forms; Fields; search

search Typ Pola
===============

Renderuje pole ``<input type="search" />``, które jest polem tekstowym
z specjalną funkcjonalnością wspieraną przez niektóre przeglądarki.

Przeczytaj na temat pola typu search na stronie `DiveIntoHTML5.org`_

+------------------+----------------------------------------------------------------------+
| Renderowane jako | ``input search`` pole                                                |
+------------------+----------------------------------------------------------------------+
| Odziedziczone    | - `max_length`_                                                      |
| opcje            | - `required`_                                                        |
|                  | - `label`_                                                           |
|                  | - `trim`_                                                            |
|                  | - `read_only`_                                                       |
|                  | - `error_bubbling`_                                                  |
+------------------+----------------------------------------------------------------------+
| Rodzic           | :doc:`text</reference/forms/types/text>`                             |
+------------------+----------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SearchType` |
+------------------+----------------------------------------------------------------------+

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. _`DiveIntoHTML5.org`: http://diveintohtml5.org/forms.html#type-search
