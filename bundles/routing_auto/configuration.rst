Configuration Reference
=======================

The RoutingAutoBundle can be configured under the ``cmf_routing_auto`` key in your
application configuration.

Configuration
-------------

``adapter``
~~~~~~~~~~~

**type**: ``scalar`` **default**: ``doctrine_phpcr_odm`` if ``persistence`` configuration option is set ``phpcr``

This defines the adapter used to manage routes.

``auto_mapping``
~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``true``

Look for the configuration file ``cmf_routing_auto.yml`` in `Resource/config` folder of all
available bundles.

``persistence``
~~~~~~~~~~~~~~~

``phpcr``
.........

.. todo