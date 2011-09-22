.. index::
   single: Forms; Fields; csrf

csrf Typ Pola
=============

Typ ``csrf`` jest ukrytym polem zawierającym token CSRF.

+------------------+--------------------------------------------------------------------+
| Renderowane jako | pole ``input`` ``hidden``                                          |
+------------------+--------------------------------------------------------------------+
| Opcje            | - ``csrf_provider``                                                |
|                  | - ``page_id``                                                      |
|                  | - ``property_path``                                                |
+------------------+--------------------------------------------------------------------+
| Rodzic           | ``hidden``                                                         |
+------------------+--------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\CsrfType` |
+------------------+--------------------------------------------------------------------+

Opcje Pola
----------

csrf_provider
~~~~~~~~~~~~~

**typ**: ``Symfony\Component\Form\CsrfProvider\CsrfProviderInterface``

Obiekt ``CsrfProviderInterface`` który potrafi generować token CSRF.
Jeśli nie jest ustawiony, pobierana jest domyślna klasa.

intention
~~~~~~~~~

**typ**: ``string``

Opcjonalny unikalny identyfikator używany do generowania tokenu CSRF.

.. include:: /reference/forms/types/options/property_path.rst.inc
