Alternate Locale Handling
=========================

The CMF provides a powerful way to persist document in different locales.
Each of those translated documents are Representations of another. In a
SEO context it would be great to show the available routes to translations
of the current representation.

Example
-------

Lets persist a document in three locales.

    // src/Acme/ApplicationBundle/DataFixtures/Doctrine/PHPCR/ExampleFixture.php

    class ExampleFeature implements FixtureInterface
    {
        /**
         * Load data fixtures with the passed EntityManager
         *
         * @param DocumentManager|ObjectManager $manager
         */
        public function load(ObjectManager $manager)
        {
            $document = new Document();
            $document->setTitle('The Title');
            $document->setBody('The body is the main content');
            $manager->persist($document);
            $manager->bindTranslation($document, 'en');

            $document->setTitle('Der Title');
            $document->setBody('Der Body ist der Content');
            $manager->bindTranslation($document, 'en');

            $document->setTitle('Le title');
            $document->setBody('Le body ... Todo: @dbu');
            $manager->bindTranslation($document, 'fr');

            $manager->flush();
        }
    }

.. note::
    To get more information about translating content by the phpcr-odm have a look
    at `translatedDocuments`_.

The mapped routes can look like ``/en/the-title``, ``/de/der-titel``
and ``/fr/le-title``. When viewing the content of one of the route, we
would expect hints how to navigate to the others. So a somethin like that should
be rendered into your head section of your DOM:

    <link rel="alternate" href="/fr/le-title" hreflang="fr" />
    <link rel="alternate" href="/de/der-titel" hreflang="de" />

Configuration
-------------

The SeoBundle serves a ``AlternateLocaleProvider`` to build alternate locale information
for a given content based on the PHPCR. You can easily enable that by

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale: ~
            persistence:
                phpcr: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale enabled="true" />
                <persistence>
                    <phpcr
                        enabled="true"
                    />
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'alternate_locale' => array (
                'enabled' => true,
            ),
            'persistence' => array(
                'phpcr' => array('enabled' => true),
            ),
        ));

You have to enable persistence by PHPCR to have the default provider available.

Create your own provider
------------------------

Cause the default provider serves the routes for the alternate locale contents directly from the
phpcr-odm. For other persistence layers or custom needs on the translated location routes you can
create your own provider by implementing the ``AlternateLocaleProviderInterface``

    /src/Acme/ApplicationBundle/AlternateLocaleProvider.php
    use Symfony\Cmf\Bundle\SeoBundle\AlternateLocaleProviderInterface;
    use Symfony\Cmf\Bundle\SeoBundle\Model\AlternateLocale;
    use Symfony\Cmf\Bundle\SeoBundle\Model\AlternateLocaleCollection;

    class AlternateLocaleProvider implements AlternateLocaleProviderInterface
    {
        /**
         * Creates a collection of AlternateLocales for one content object.
         *
         * @param object $content
         *
         * @return AlternateLocaleCollection
         */
        public function createForContent($content)
        {
            $alternateLocaleCollection = new AlternateLocaleCollection();
            // get the alternate locales for the given content
            $alternateLocales = CustomLocaleHelper->getAllForContent($content);

            // add the alternate locales except the current one
            $currentLocale = $content->getLocale();
            foreach ($alternateLocales as $locale) {
                if ($locale === $currentLocale) {
                    continue;
                }

                $alternateLocaleCollection->add(
                    new AlternateLocale(
                        $this->urlGenerator->generate($content, array('_locale' => $locale), true),
                        $locale
                    )
                );
            }

            return $alternateLocaleCollection;
        }

        /**
         * Creates a collection of AlternateLocales for many content object.
         *
         * @param array|object[] $contents
         *
         * @return AlternateLocaleCollection[]
         */
        public function createForContents(array $contents)
        {
            $result = array();
            foreach ($contents as $content) {
                $result[] = $this->createForContent($content);
            }

            return $result;
        }
    }

Create a service for your provider


.. configuration-block::

    .. code-block:: yaml
        services:
            acme.application.alternate_locale_provider
                class: "Acme\ApplicationBundle\AlternateLocaleProvider"

    .. code-block:: xml
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme.application.alternate_locale_provider" class="Acme\ApplicationBundle\AlternateLocaleProvider">
                </service>
            </services>

        </container>

    .. code-block:: php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('acme.application.alternate_locale_provider', new Definition(
            'Acme\ApplicationBundle\AlternateLocaleProvider'
        ));

Now you have to inform ``CmfSeoBundle`` to use your custom alternate locale provider instead of the default one.
You can do so by setting the ``provider_id`` to the service id you have just created.


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale:
                provider_id: acme.application.alternate_locale_provider

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale provider-id="acme.application.alternate_locale_provider" />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'alternate_locale' => array (
                'provider_id' => acme.application.alternate_locale_provider,
            ),
        ));

Alternate locales on Sitemap
----------------------------

.. note::
    The Sitemap was introduced in Version 1.2

You can also provide alternate locale information on your Sitemap creted by the ``CmfSeoBundle``.
To do so only have to activate the ``alternate_locale`` insider your configuration. There is an
``AlternateLocalesGuesser`` which uses the ``AlternateLocaleProvider`` to add the alternate
locale content to the so called ``UrlInformation``, which are rendered into the Sitemap like:

    <?xml version="1.0"?>
    <urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9" xmlns:xhtml="http://www.w3.org/1999/xhtml">
     <url>
         <loc>http://www.your-domain.org/en/the-title</loc>
         <changefreq>never</changefreq>
         <xhtml:link href="http://www.your-domain.org/de/der-title" hreflang="de" rel="alternate"/>
         <xhtml:link href="http://www.your-domain.org/fr/le-title" hreflang="fr" rel="alternate"/>
     </url>
    </urlset>

For more information about the sitmap creation of the ``CmfSeoBundle`` have a look at `Sitemap`_.

.. _`translatedDocuments`: ../phpcr-odm/multilang
.. _`Sitemap`: sitemap
