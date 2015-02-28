Configuration Reference
=======================

The SeoBundle can be configured under the ``cmf_seo`` key in your application
configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/seo`` namespace.

Configuration
-------------

persistence
~~~~~~~~~~~

phpcr
"""""

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            persistence:
                phpcr:
                    enabled: false
                    manager_name: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                    />
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'manager_name' => null,
                ),
            ),
        ));

enabled
*******

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

manager_name
************

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

translation_domain
~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``messages``

The translation domain to use when translating the title and description
template. See :ref:`bundles-seo-title-description-template` for more
information.


title
~~~~~

**type**: ``string`` **default**: ``null``

The title template, read :ref:`here <bundles-seo-title-description-template>`
about the usage.

description
~~~~~~~~~~~

**type**: ``string`` **default**: ``null``

The description template, read :ref:`here <bundles-seo-title-description-template>`
about the usage.

original_route_pattern
~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``canonical``

The original route strategy to use when multiple routes have the same content.
Can be one of ``canonical`` or ``redirect``.

content_key
~~~~~~~~~~~

**type**: ``string`` **default**: ``null`` (or ``DynamicRouter::CONTENT_KEY`` when RoutingBundle is enabled)

The name of the Request attribute which contains the content object. This is
required when the RoutingBundle is not enabled, otherwise it defaults to
``DynamicRouter::CONTENT_KEY`` (which evaluates to ``contentDocument``).

sonata_admin_extension
~~~~~~~~~~~~~~~~~~~~~~

If set to ``true``, the Sonata Admin Extension provided by the SeoBundle is
activated.

enabled
"""""""

**type**: ``enum`` **valid values** ``true|false|auto`` **default**: ``auto``

If ``true``, the Sonata Admin Extension will be activated. If set to ``auto``,
it is activated only if the SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

form_group
""""""""""

**type**: ``string`` **default**: ``form.group_seo``

The name of the form group of the group provided by the Sonata Admin
Extension.

``enable_content_listener``
"""""""""""""""""""""""""""

.. versionadded:: 1.2
    The ``enable_content_listener`` configuration key was introduced in SeoBundle 1.2

**type**: ``boolean`` **default**: ``true``

Whether or not the ``Symfony\Cmf\Bundle\SeoBundle\EventListener\ContentListener`` should be loaded
as a ``kernel.request`` listener. The ContentListener is responsible for extracting SEO data from
CMF content documents. Set this to false to disable the listener. If you want to use your own content listener
you will need to register your listener as a service and tag it as a ``kernel.request`` listener.
