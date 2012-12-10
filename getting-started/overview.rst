Overview
========

This guide will help you understand the basic parts of Symfony CMF and how 
they work together to provide the default pages you can see when browsing
the Symfony CMF Standard Edition (SE) installation.

It assumes you have already installed Symfony CMF and have carefully
read the Symfony2 book.

AcmeMainBundle and SimpleCMSBundle
----------------------------------

Symfony CMF SE comes with a default AcmeMainBundle to help you get started,
in a similar way that Symfony2 has AcmeDemoBundle, providing you some
demo pages visible on your browser. However, AcmeMainBundle doesn't include
controllers or configuration files, like you probably would expect. It contains
little more than a twig file and `Fixtures <http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html>`_
data, that was loaded into your database during installation.

There are several bundles working together in order to turn the fixture data
into a browsable website. The overall, simplified process is:

- When a request is received, the Symfony CMF :doc:`routing`'s Dynamic Router is used to handle the incoming request.
- The Dynamic Router is able to match the requested URL with a specific ContentBundle's Content stored in the database.
- The retrieved content's information is used to determine which controller to pass it on to, and which template to use. 
- As configured, the retrieved content is passed to ContentBundle's ContentController, which will handle it and render AcmeMainBundle's layout.html.twig.

 Again, this is simplified view of a very simple CMS built on top of Symfony CMF.
 To fully understand all the possibilities of the CMF, a carefull look into
 each component is needed.