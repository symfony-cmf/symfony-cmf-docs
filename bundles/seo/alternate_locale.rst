Alternate Locale Handling
=========================

The CMF provides a powerful way to persist document in different locales.
Each of those translated documents are representations of another. In a
SEO context it would be great to show the available routes to translations
of the current representation.

Example
-------

Lets persist a document in three locales.::

    // src/Acme/ApplicationBundle/DataFixtures/Doctrine/PHPCR/ExampleFixture.php

    <?php

    namespace AppBundle\DataFixtures\PHPCR;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\ODM\PHPCR\DocumentManager;
    use Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent;
    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;

    /**
     * @author Maximilian Berghoff <Maximilian.Berghoff@mayflower.de>
     */
    class ExampleFeature implements FixtureInterface
    {
        /**
         * Load data fixtures with the passed EntityManager
         *
         * @param DocumentManager|ObjectManager $manager
         */
        public function load(ObjectManager $manager)
        {
            $parent = $manager->find(null, '/cms/routes');

            $document = new StaticContent();
            $document->setTitle('The Title');
            $document->setBody('The body is the main content');
            $manager->persist($document);
            $manager->bindTranslation($document, 'en');
            $route = new Route();
            $route->setPosition($parent, 'en');
            $route->setCondition($document);
            $manager->persist($route);

            $document->setTitle('Der Titel');
            $document->setBody('Der Body ist der Content');
            $manager->bindTranslation($document, 'en');
            $route = new Route();
            $route->setPosition($parent, 'de');
            $route->setCondition($document);
            $manager->persist($route);

            $document->setTitle('Le titre');
            $document->setBody('Le contenu principale');
            $manager->bindTranslation($document, 'fr');
            $route = new Route();
            $route->setPosition($parent, 'fr');
            $route->setCondition($document);
            $manager->persist($route);

            $manager->flush();
        }
    }

This creates a content document for the languages german, english and french and its routes.
So the routes are persisted as objects, but the content is available under the following urls:

+--------------------+---------------------+
| locale             | url                 |
+================================+=========+
| ``english``        | ``/en/the-title``   |
+--------------------+---------------------+
| ``german``         | ``/de/der-title``   |
+--------------------+---------------------+
| ``french``         | ``/fr/le-titre``    |
+--------------------+---------------------+

- english: /en/
- german: /de/der-titel

.. note::
    To get more information about translating content by the PHPCR-ODM have a look
    at :doc:`PHPCR-ODM multilang<phpcr_odm/multilang>`.

When viewing the page in one language, we would expect hints how to navigate to the others.
Search engines expect the following in the ``<head>`` section of the page:

    <link rel="alternate" href="/fr/le-titre" hreflang="fr" />
    <link rel="alternate" href="/de/der-titel" hreflang="de" />

Configuration
-------------

The SeoBundle serves a ``AlternateLocaleProvider`` to build alternate locale information
for a given content based on the PHPCR-ODM. You can easily enable that by:

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

The default provider serves the routes for the alternate locale contents directly from the
PHPCR-ODM. For other persistence layers or custom needs on the translated location URLs you can
create your own provider by implementing the ``AlternateLocaleProviderInterface``::

    // src/Acme/ApplicationBundle/AlternateLocaleProvider.php

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
            $alternateLocales = $this->getAllForContent($content);

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

        /**
         * Creates a list of locales the content is also persisd
         *
         * @var object $content
         * @return array The list of locales
         */
        public function getAllForContent($content)
        {
            $list = array();
            // implement you logic

            return $list;
        }

    }

Create a service for your provider:

.. configuration-block::

    .. code-block:: yaml
        services:
            acme.application.alternate_locale.provider
                class: "Acme\ApplicationBundle\AlternateLocaleProvider"

    .. code-block:: xml
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme.application.alternate_locale.provider" class="Acme\ApplicationBundle\AlternateLocaleProvider">
                </service>
            </services>

        </container>

    .. code-block:: php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('acme.application.alternate_locale.provider', new Definition(
            'Acme\ApplicationBundle\AlternateLocaleProvider'
        ));

Now you have to configure ``CmfSeoBundle`` to use your custom alternate locale provider instead of the default one.
Set the ``alternate_locale.provider_id``  to the service you just created:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            alternate_locale:
                provider_id: acme.application.alternate_locale.provider

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo">
                <alternate-locale provider-id="acme.application.alternate_locale.provider" />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'alternate_locale' => array (
                'provider_id' => acme.application.alternate_locale.provider,
            ),
        ));

.. versionadded:: 1.2
    For activated :doc:`sitemap<seo/sitemap>` the alternate locales will be pushed into the sitemap too.
