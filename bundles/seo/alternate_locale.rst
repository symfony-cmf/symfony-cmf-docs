Alternate Locale Handling
=========================

.. versionadded:: 1.1
    Support for handling alternate locales was added in SeoBundle version 1.1.0.

Alternate locales are a way of telling search engines how to find translations
of the current page. The SeoBundle provides a way to manage alternate locales
and render them together with the other SEO information.

When the alternate locale handling is set up and found alternates, you will
find links like the following in the ``<head>`` part of your HTML pages:

.. code-block:: html

    <link rel="alternate" href="/fr/le-titre" hreflang="fr">
    <link rel="alternate" href="/de/der-titel" hreflang="de">

When using PHPCR-ODM, there is almost no work to do, as the bundle can use the
Doctrine meta data to figure out which translations exists for a content. More
information on translating content with the PHPCR-ODM is in the chapter
:doc:`Doctrine PHPCR-ODM Multilanguage Support <../phpcr_odm/multilang>`.

Setting Up Alternate Locale Support
-----------------------------------

Enable alternate locale support:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'alternate_locale' => true,
        ));

If you are using PHPCR-ODM, enabling ``phpcr: ~`` in the seo bundle
configuration will activate a listener that extracts the alternate locales
from the PHPCR-ODM meta data. For other storage systems, you will need to
write a provider and configure the bundle to use that provider - see below.

Rendering Alternate Locales
---------------------------

The alternate locales are rendered together with the other SEO metadata by the
twig function ``sonata_seo_metadatas``.

Creating a Custom Alternate Locales Provider
--------------------------------------------

The alternate locale provider is asked to provide translated URLs for a content
object. The bundle comes with a provider for PHPCR-ODM. For other persistence
layers or custom requirements on the translated URLs you need to create your
own provider implementing the ``AlternateLocaleProviderInterface``. For some
inspiration, have a look at
``Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\AlternateLocaleProvider``.

Define a service for your provider, so that you can configure the seo bundle to
use your custom alternate locale provider instead of the default one. Set the
``alternate_locale.provider_id`` to the service you just created:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale:
                provider_id: alternate_locale.provider

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale provider-id="alternate_locale.provider" />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'alternate_locale' => array (
                'provider_id' => 'alternate_locale.provider',
            ),
        ));

.. versionadded:: 1.2
    When :doc:`Sitemaps <sitemap>` are enabled, alternate locales are also
    added to the Sitemap.
