- [ ] Change the way the batch randerer works. It shouldn't allocated more memory.
      Once the VertexBuffer fills up flush the data onto the screen reset the buffer
      and continue reading.
