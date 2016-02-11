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

    use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;

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

Now you can set some SEO data for this ``Page`` using the metadata::

    use Acme\SiteBundle\Document\Page;
    use Symfony\Cmf\Bundle\SeoBundle\SeoMetadata;

    $page = new Page();
    // ... set some page properties

    $pageMetadata = new SeoMetadata();
    $pageMetadata->setDescription('A special SEO description.');
    $pageMetadata->setTags('seo, cmf, symfony');

    $page->setSeoMetadata($pageMetadata);

Doctrine PHPCR-ODM Integration
------------------------------

In order to easily persist the SeoMetadata when using Doctrine PHPCR-ODM, the
SeoBundle provides a special ``SeoMetadata`` document with the correct
mappings. This document should be mapped as a child of the content document.

To be able to use this document, you have to enable the PHPCR persistence:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            persistence:
                phpcr: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/seo">
            <persistence>
                <phpcr />
            </persistence>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', array(
            'persistence' => array(
                'phpcr' => true,
            ),
        ));

.. tip::

    This is not needed if you already enabled PHPCR on the ``cmf_core``
    bundle. See :doc:`the CoreBundle docs <../core/persistence>` for more
    information.

After you've enabled PHPCR, map your seoMetadata as a child:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/SiteBundle/Document/Page.php
        namespace Acme\SiteBundle\Document;

        use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;
        use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

        /**
         * @PHPCR\Document()
         */
        class Page implements SeoAwareInterface
        {
            /**
             * @PHPCR\Child
             */
            protected $seoMetadata;

            // ...
        }

    .. code-block:: yaml

        # src/Acme/SiteBundle/Resources/config/doctrine/Page.odm.yml
        Acme\SiteBundle\Document\Page:
            # ...
            child:
                # ...
                seoMetadata: ~

    .. code-block:: xml

        <!-- src/Acme/SiteBundle/Resources/config/doctrine/Page.odm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping
            xmlns="http://doctrine-project.org/schemas/phpcr-odm/phpcr-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/phpcr-odm/phpcr-mapping
            https://github.com/doctrine/phpcr-odm/raw/master/doctrine-phpcr-odm-mapping.xsd"
        >
            <document name="Acme\SiteBundle\Document\Page">
                <!-- ... -->
                <child name="seoMetadata" />
            </document>
        </doctrine-mapping>

And after that, you can use the
``Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoMetadata`` document::

    // src/Acme/SiteBundle/DataFixture/PHPCR/LoadPageData.php
    namespace Acme\SiteBundle\DataFixtures\PHPCR;

    use Acme\SiteBundle\Document\Page;
    use Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoMetadata;
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

Doctrine ORM
------------

You can also use the Doctrine ORM with the CmfSeoBundle. You can just use the
``Symfony\Cmf\Bundle\SeoBundle\SeoMetadata`` class and map it as an
object:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/SiteBundle/Entity/Page.php
        namespace Acme\SiteBundle\Entity;

        use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;
        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity()
         */
        class Page implements SeoAwareInterface
        {
            /**
             * @ORM\Column(type="object")
             */
            protected $seoMetadata;

            // ...
        }

    .. code-block:: yaml

        # src/Acme/SiteBundle/Resources/config/doctrine/Page.orm.yml
        Acme\SiteBundle\Entity\Page:
            # ...
            fields:
                # ...
                seoMetadata:
                    type: object

    .. code-block:: xml

        <!-- src/Acme/SiteBundle/Resources/config/doctrine/Page.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\SiteBundle\Entity\Page">
                <!-- ... -->
                <field name="seoMetadata" type="object" />
            </entity>
        </doctrine-mapping>

You can also choose to put the ``SeoMetadata`` class into a seperate table. To
do this, you have to enable ORM support just like you enabled PHPCR enabled
above and add a OneToOne or ManyToOne relation between the content entity and
the ``SeoMetadata`` entity.

Form Type
---------

The bundle also provides a special form type called ``seo_metadata``. This
form type can be used in forms to edit the ``SeoMetadata`` object.

.. caution::

    The bundles requires the `BurgovKeyValueFormBundle`_ when using the form
    type. Make sure you install and enable it.

Sonata Admin Integration
------------------------

Besides providing a form type, the bundle also provides a Sonata Admin
Extension. This extension adds a field for the ``SeoMetadata`` when an admin
edits an objec that implements the ``SeoAwareInterface`` in the Sonata Admin
panel.

.. caution::

    The Sonata Admin uses the Form Type provided by the CmfSeoBundle, make
    sure you have the `BurgovKeyValueFormBundle`_ installed.

.. _`BurgovKeyValueFormBundle`: https://github.com/Burgov/KeyValueFormBundle
