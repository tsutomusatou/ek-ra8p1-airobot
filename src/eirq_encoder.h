#ifndef EIRQ_ENCODER_H_
#define EIRQ_ENCODER_H_

extern void encoder_irq_init(void);
extern volatile int32_t encoder_left;
extern volatile int32_t encoder_right;

#endif /* EIRQ_ENCODER_H_ */
