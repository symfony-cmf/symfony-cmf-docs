Create your own Blocks
======================

Follow these steps to create a block:

* create a block document;
* create a block service and declare it (optional);
* create a data object representing your block in the repository, see
  :ref:`bundle-block-document`;
* render the block, see :ref:`bundle-block-rendering`;

Lets say you are working on a project where you have to integrate data
received from several RSS feeds.  Of course you could create an ActionBlock
for each of these feeds, but wouldn't this be silly? In fact all those actions
would look similar: Receive data from a feed, sanitize it and pass the data to
a template. So instead you decide to create your own block, the RSSBlock.

Create a block document
-----------------------

The first thing you need is an document that contains the data. It is
recommended to extend ``Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock``
contained in this bundle (however you are not forced to do so, as long as you
implement ``Sonata\BlockBundle\Model\BlockInterface``). In your document, you
need to define the ``getType`` method which just returns ``acme_main.block.rss``.

.. code-block:: php

    // src/Acme/MainBundle/Document/RssBlock.php
    namespace Acme\MainBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCRODM;
    use Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock;

    /**
     * Rss Block
     *
     * @PHPCRODM\Document(referenceable=true)
     */
    class RssBlock extends BaseBlock
    {
        public function getType()
        {
            return 'acme_main.block.rss';
        }
    }

Create a Block Service
----------------------

You could choose to use a an already existing block service because the
configuration and logic already satisfy your needs. For our rss block we
create a service that knows how to handle RSSBlocks:

* The method ``getDefaultSettings`` configures a title, url and the maximum
  amount of items::

      // ...
      public function getDefaultSettings()
      {
          return array(
              'url'      => false,
              'title'    => 'Insert the rss title',
              'maxItems' => 10,
          );
      }
      // ...

* The execute method passes the settings to an rss reader service and forwards
* The feed items to a template, see :ref:`bundle-block-execute`

Make sure you implement the interface
``Sonata\BlockBundle\Block\BlockServiceInterface`` or an existing block
service like ``Sonata\BlockBundle\Block\BaseBlockService``.

Define the service in a config file. It is important to tag your BlockService
with ``sonata.block``, otherwise it will not be known by the Bundle.

.. configuration-block::

    .. code-block:: yaml

        acme_main.rss_reader:
            class: Acme\MainBundle\Feed\SimpleReader

        sandbox_main.block.rss:
            class: Acme\MainBundle\Block\RssBlockService
            arguments:
                - "acme_main.block.rss"
                - "@templating"
                - "@sonata.block.renderer"
                - "@acme_main.rss_reader"
            tags:
                - {name: "sonata.block"}

    .. code-block:: xml

        <service id="acme_main.rss_reader" class="Acme\MainBundle\Feed\SimpleReader" />

        <service id="sandbox_main.block.rss" class="Acme\MainBundle\Block\RssBlockService">
            <tag name="sonata.block" />

            <argument>acme_main.block.rss</argument>
            <argument type="service" id="templating" />
            <argument type="service" id="sonata.block.renderer" />
            <argument type="service" id="acme_main.block.rss_reader" />
        </service>
