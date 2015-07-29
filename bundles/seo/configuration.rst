Configuration Reference
=======================

The SeoBundle can be configured under the ``cmf_seo`` key in your application
configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/seo`` namespace.

Configuration
-------------

``persistence``
~~~~~~~~~~~~~~~

``phpcr``
"""""""""

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

``enabled``
***********

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

``manager_name``
****************

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

``translation_domain``
~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``messages``

The translation domain to use when translating the title and description
template. See :ref:`bundles-seo-title-description-template` for more
information.

``title``
~~~~~~~~~

**type**: ``string`` **default**: ``null``

The title template, read :ref:`here <bundles-seo-title-description-template>`
about the usage.

``description``
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``null``

The description template, read :ref:`here <bundles-seo-title-description-template>`
about the usage.

``original_route_pattern``
~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``canonical``

The original route strategy to use when multiple routes have the same content.
Can be one of ``canonical`` or ``redirect``.

``content_listener``
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.2
    The ``content_listener`` configuration key was introduced in SeoBundle 1.2.

``enabled``
"""""""""""

**type**: ``boolean`` **default**: ``true``

Whether or not the :ref:`bundles-seo-content-listener` should be loaded.

``content_key``
"""""""""""""""

**type**: ``string`` **default**: ``null`` (or ``DynamicRouter::CONTENT_KEY`` when RoutingBundle is enabled)

The name of the request attribute which contains the content object. This is
used by the ContentListener to extract SEO information automatically. If the
RoutingBundle is present, this defaults to ``DynamicRouter::CONTENT_KEY``
(which evaluates to ``contentDocument``), otherwise you must define this
manually or disable the content listener.

.. versionadded:: 1.2
    In versions of the SeoBundle prior to 1.2, the ``content_key`` was
    configured directly in the ``cmf_seo`` root.

``sitemap``
~~~~~~~~~~~

.. versionadded:: 1.2
    Support for sitemaps was introduced in version 1.2 of the SeoBundle.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            sitemap:
                enabled: true
                defaults:
                    default_change_frequency: always
                    templates:
                        html: CmfSeoBundle:Sitemap:index.html.twig
                        xml: CmfSeoBundle:Sitemap:index.xml.twig
                configurations:
                    sitemap: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/cmf_seo">
                <sitemap enabled="true">
                    <defaults>
                        <template format="html">CmfSeoBundle:Sitemap:index.html.twig</template>
                        <template format="xml">CmfSeoBundle:Sitemap:index.xml.twig</template>
                    </defaults>
                    <configuration name="sitemap"/>
                </sitemap>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', array(
            'sitemap' => array(
                'enabled' => true,
                'defaults' => array(
                    'templates' => array(
                        'html' => 'CmfSeoBundle:Sitemap:index.html.twig',
                        'xml' => 'CmfSeoBundle:Sitemap:index.xml.twig',
                    ),
                ),
                'configurations' => array(
                    'sitemap' => null,
                ),
            ),
        ));

``enabled``
"""""""""""

**type**: ``boolean`` **default**: ``false``

Whether or not the :doc:`sitemap` should be loaded. As soon as you configure
anything else in the ``sitemap`` section, this defaults to true.

``defaults``
""""""""""""

Contains default configuration that applies to all sitemaps.

``configurations``
""""""""""""""""""

Contains the list of sitemaps that should exist. Each sitemap can overwrite
default configuration. If not specified, a sitemap called "sitemap" exists.

``default_change_frequency``
****************************

**type**: ``enum`` **default**: ``always`` **allowed values**: 'always', 'hourly', 'daily', 'weekly', 'monthly', 'yearly', 'never'

Specify the change frequency for UrlInformation that do not have one explicitly
set.

``templates``
*************

**type**: ``hashmap`` **default**: templates for html and xml

This hashmap specifies which template to use for the sitemap in each format.
By default, you have:

* html: CmfSeoBundle:Sitemap:index.html.twig
* xml: CmfSeoBundle:Sitemap:index.xml.twig

``sonata_admin_extension``
~~~~~~~~~~~~~~~~~~~~~~~~~~

If set to ``true``, the Sonata Admin Extension provided by the SeoBundle is
activated.

``enabled``
"""""""""""

**type**: ``enum`` **valid values** ``true|false|auto`` **default**: ``auto``

If ``true``, the Sonata Admin Extension will be activated. If set to ``auto``,
it is activated only if the SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

``form_group``
""""""""""""""

**type**: ``string`` **default**: ``form.group_seo``

The name of the form group of the group provided by the Sonata Admin
Extension.

``alternate_locale``
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.1
    Support for alternate locales were added in version 1.1 of the SeoBundle

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale:
                enabled: true
                provider_id: acme.application.alternate_locale.provider

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale  enabled="true" provider-id="acme.application.alternate_locale.provider" />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'alternate_locale' => array (
                'enabled' => true,
                'provider_id' => acme.application.alternate_locale.provider,
            ),
        ));

``enabled``
"""""""""""

**type**: ``boolean`` **default**: ``true``

Whether or not the the :ref:`bundles-seo-alternate-locale` should be loaded

``provider_id``
"""""""""""""""

**type**: ``string`` **default**: ``null``

Define your a custom :doc:`AlternateLocaleProvider<../seo/sitemap>`
