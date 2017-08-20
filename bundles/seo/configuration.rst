Configuration Reference
=======================

The SeoBundle is configured under the ``cmf_seo`` key in your application
configuration. When using XML, use the
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

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'persistence' => [
                'phpcr' => [
                    'enabled' => false,
                    'manager_name' => null,
                ],
            ],
        ]);

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

**type**: ``string`` **default**: ``canonical`` **allowed values**: ``canonical`` | ``redirect``

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

For details on the meaning of the sitemap configuration, see the
:doc:`sitemap section <sitemap>`.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            sitemap:
                enabled: true
                defaults:
                    default_change_frequency: always
                    templates:
                        html: :sitemap/index.html.twig
                        xml: ::sitemap:index.xml.twig
                    loaders:
                        - _all
                    guessers:
                        - _all
                    voters:
                        - _all
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
                        <loader>_all</loader>
                        <guesser>_all</guesser>
                        <voter>_all</voter>
                    </defaults>
                    <configuration name="sitemap"/>
                </sitemap>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'sitemap' => [
                'enabled' => true,
                'defaults' => [
                    'templates' => [
                        'html' => 'CmfSeoBundle:Sitemap:index.html.twig',
                        'xml' => 'CmfSeoBundle:Sitemap:index.xml.twig',
                    ],
                    'loaders' => ['_all'],
                    'guessers' => ['_all'],
                    'voters' => ['_all'],
                ],
                'configurations' => [
                    'sitemap' => null,
                ],
            ],
        ]);

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

``loaders``
***********

**type**: ``array`` **default**: [_all]

Limit which of the loaders should be used for this sitemap.

``guessers``
************

**type**: ``array`` **default**: [_all]

``voters``
**********

**type**: ``array`` **default**: [_all]

``form``
~~~~~~~~

``data_class``
""""""""""""""

``seo_metadata``
****************

.. versionadded:: 1.2
    The ``seo_metadata`` setting was introduced in version 1.2.

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata``

Configures the class to use when creating new ``SeoMetadata`` objects using the
:ref:`SeoMetadata form type <bundles-seo-metadata-form-type>`.

When the :doc:`PHPCR-ODM <../phpcr_odm/introduction>` persistence layer is enabled,
this defaults to ``Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoMetadata``.

.. _bundles-seo-config-error:

error
~~~~~

.. versionadded:: 1.2
    The ``error`` settings were introduced in SeoBundle 1.2.

.. seealso::

    Learn more about error pages in ":doc:`error_pages`".

enable_parent_provider
""""""""""""""""""""""

**type**: ``boolean`` **default**: ``false``

Whether the parent suggestion provider should be enabled.

enable_sibling_provider
"""""""""""""""""""""""

**type**: ``boolean`` **default**: ``false``

Whether the sibling suggestion provider should be enabled.

templates
"""""""""

**type**: ``array`` **default**: ``{ html: 'CmfSeoBundle:Exception:error.html.twig' }``

A list of templates to use for the custom error page. By default, only the HTML
format is configured. The default Symfony exception controller will be used for
the other formats.

exclusion_rules
"""""""""""""""

**type**: ``array``

To specify exclusion rules for pages that shouldn't be handled by the custom
exception controller. In these cases, the default Symfony exception controller
will be used instead.

Exclusion rules allow to match the following fields:

* ``path``
* ``host``
* ``methods``
* ``ips``

For instance, to not use the special exception controller for the ``/admin``
routes, use:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            error:
                exclusion_rules:
                    - { path: ^/admin }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://cmf.symfony.com/schema/dic/seo http://cmf.symfony.com/schema/dic/seo/seo-1.0.xsd"
        >

            <config xmlns="http://symfony.com/schema/dic/seo">
                <error>
                    <exclusion-rule path="^/admin" />
                </error>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'error' => [
                'exclusion_rules' => [
                    ['path' => '^/admin'],
                ],
            ],
        ]);

``alternate_locale``
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.1
    Support for alternate locales was added in version 1.1 of the SeoBundle.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale:
                enabled: true
                provider_id: app.alternate_locale.provider

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale  enabled="true" provider-id="app.alternate_locale.provider" />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'alternate_locale' => [
                'enabled' => true,
                'provider_id' => app.alternate_locale.provider,
            ],
        ]);

``enabled``
"""""""""""

**type**: ``boolean`` **default**: ``true``

Whether or not the the :ref:`bundles-seo-alternate-locale` should be loaded

``provider_id``
"""""""""""""""

**type**: ``string`` **default**: ``null``

Specify the service id of a custom :doc:`AlternateLocaleProvider <../seo/alternate_locale>`.
