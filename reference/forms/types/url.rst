.. index::
   single: Forms; Fields; url

url Typ Pola
============

Pole ``url`` jest polem tekstowym które poprzedza przesłaną wartość danym protokołem (np. ``http://``),
jeśli przesłana wartość nie posiada jeszcze protokołu.

+------------------+-------------------------------------------------------------------+
| Renderowane jako | ``input url`` pole                                                |
+------------------+-------------------------------------------------------------------+
| Opcje            | - ``default_protocol``                                            |
+------------------+-------------------------------------------------------------------+
| Odziedziczone    | - `max_length`_                                                   |
| opcje            | - `required`_                                                     |
|                  | - `label`_                                                        |
|                  | - `trim`_                                                         |
|                  | - `read_only`_                                                    |
|                  | - `error_bubbling`_                                               |
+------------------+-------------------------------------------------------------------+
| Rodzic           | :doc:`text</reference/forms/types/text>`                          |
+------------------+-------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\UrlType` |
+------------------+-------------------------------------------------------------------+

Opcje Pola
----------

default_protocol
~~~~~~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``http``

Jeśli przesłana wartość nie rozpoczyna się od protokołu (np. ``http://``, ``ftp://``, etc.),
protokół ten zostanie dodany do ciągu znaków gdy dane są dołączane do formularza.

Odziedziczone Opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
