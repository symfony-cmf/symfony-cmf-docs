Saving the SeoMetadata in the Object
====================================

The ``SeoMetadata`` can be saved in the object, so you can persist it into the
database. This option gives admins the possiblity of changing the SEO data for
the document.

In order to save the ``SeoMetadata`` in the object, the object should
implement the ``SeoAwareInterface``. This requires a getter and a setter for
the ``SeoMetadata``::

    // src/Acme/SiteBundle/Document/Page.php
    namespace Acme\SiteBundle\Document;

    use Symfony\Cmf\Bundle\SeoBundle\Model\SeoAwareInterface;

    class Page implements SeoAwareInterface
    {
        protected $seoMetadata;

        // ...
        public function getSeoMetadata()
        {
            return $this->seoMetadata;
        }
        
        public function setSeoMetadata($metadata)
        {
            $this->seoMetadata = $metadata;
        }
    }

Now you can set some SEO data for this ``Page`` using the metadata. For
instance inside a data fixture::

    // src/Acme/SiteBundle/DataFixture/PHPCR/LoadPageData.php
    namespace Acme\SiteBundle\DataFixtures\PHPCR;

    use Acme\SiteBundle\Document\Page;
    use Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\Common\DataFixtures\FixtureInterface;

    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $manager)
        {
            $page = new Page();
            // ... set some page properties

            $pageMetadata = new SeoMetadata();
            $pageMetadata->setDescription('A special SEO description.');
            $pageMetadata->setTags('seo, cmf, symfony');

            $page->setSeoMetadata($pageMetadata);

            $manager->persist($page);
            $manager->flush();
        }
    }

.. tip::

    While this examples shows a Doctrine PHPCR ODM data fixture, the bundle
    works fully storage agnostic. You can use it with every storage system you
    like.

Persisting the SeoMetadata with Doctrine
----------------------------------------

Since Doctrine doesn't allow you to persist an object as a database field,
there is a problem. The bundle provides a solution to solve this by providing
a Doctrine Listener. This listener will serialize the metadata object when
persisting it and it'll unserialize the metadata when the object is fetched
from the database.

In order to use this listener, you should activate either ``orm`` or ``phpcr``
as persisting layer for the SeoBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            persistence:
                phpcr: true
                # when using the ORM:
                # orm: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/seo">
            <persistence>
                <phpcr />
                <!-- when using the ORM:
                <orm />
                -->
            </persistence>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', array(
            'persistence' => array(
                'phpcr' => true,
                // when using the ORM:
                // 'orm' => true,
            ),
        ));

This will automatically enable the listener. If you don't want to enable the
listener, but you want to enable a persistence layer, you can set the
``metadata_listener`` option to ``false``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            persistence:
                # ...
            metadata_listener: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/seo"
            metadata-listener="false"
        >
            <persistence>
                <!-- ... -->
            </persistence>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', array(
            'persistence' => array(
                // ...
            ),
            'metadata_listener' => false,
        ));

Form Type
---------

The bundle also provides a special form type called ``seo_metadata``. This
form type can be used in forms to edit the ``SeoMetadata`` object.

Sonata Admin Integration
------------------------

Besides providing a form type, the bundle also provides a Sonata Admin
Extension. This extension adds a field for the ``SeoMetadata`` when an admin
edits an objec that implements the ``SeoAwareInterface`` in the Sonata Admin
panel.
