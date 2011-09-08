.. index::
   single: Configuration Reference; WebProfiler

WebProfilerBundle - Konfiguracja
===============================

Pełna Domyślna Konfiguracja
---------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # wyświetlaj informacje pomocnicze w celu zmniejszenia toolbara
            verbose:             true

            # wyświetlaj web debug toolbar na dole stron wraz z podsumowaniem profilera
            toolbar:             false

            # daj sobie możliwość do zobaczenia zebranych danych przed przekierowaniem
            intercept_redirects: false
