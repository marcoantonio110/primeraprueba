// $Id: slideshow.js,v 1.9.2.1.2.1 2009/04/05 17:38:34 timcn Exp $

Drupal.behaviors.slideshow = function(context) {
  $('.slideshow', context).each(function() {
    new Drupal.slideshow(this);
  });
};

Drupal.imageQueue = new function() {
  this.processing = false;
  this.queue = [];
  var obj = this;

  this.add = function(url) {
    this.queue.push(url);
    if (!this.processing) this.process();
  };

  this.process = function() {
    this.processing = this.queue.length > 0;
    if (this.processing) {
      var x = $('<img />').attr('src', this.queue.shift()).load(function() { obj.process(); });
    }
  };

  return this;
};

Drupal.slideshow = function(container) {
  this.container = $(container);
  this.id = container.id;
  $.extend(this, Drupal.settings.slideshow[this.id]);

  var slideshow = this;
  this.container
    .find('.previous').click(function() {
      slideshow.previous();
      return false;
    }).end()
    .find('.next, img').click(function() {
      slideshow.next();
      return false;
    }).end();

  for (var i = 0; i < this.images.length; i++) {
    Drupal.imageQueue.add(this.images[i].src);
  }
};

Drupal.slideshow.prototype = {
  previous: function() {
    if (--this.current < 0) this.current = this.images.length - 1;
    this.update();
  },

  next: function() {
    if (++this.current >= this.images.length) this.current = 0;
    this.update();
  },

  update: function() {
    var current = this.images[this.current];

    this.container
      .find('img').attr('src', current.src).end()
      .find('.title').html(current.title).end()
      .find('.current').html(this.current + 1).end()
      .find('.next').attr('href', this.link.replace('$slideshow$', 1 + (this.current + 1 >= this.images.length ? 0 : this.current + 1))).end()
      .find('.previous').attr('href', this.link.replace('$slideshow$', (this.current < 1  ? this.images.length : this.current))).end();
  }
};
